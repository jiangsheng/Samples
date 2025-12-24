using Microsoft.Office.Interop.Outlook;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using Outlook= Microsoft.Office.Interop.Outlook;

namespace OutlookSenderStatistics
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }
        Outlook.Application outlookApp;
        List<InboxSelection> inboxSelections = new List<InboxSelection>();
        Type? outlookType = Type.GetTypeFromProgID("Outlook.Application", true);
        private void toolStripButtonAttachToOutlook_Click(object sender, EventArgs e)
        {
            if (outlookApp != null) return;

            try
            {
                outlookApp = Activator.CreateInstance(outlookType) as Outlook.Application;
                if (outlookApp != null)
                {
                    inboxSelections.Clear();
                    var ns = outlookApp.GetNamespace("MAPI");
                    var mailFolders = ns.Folders;
                    foreach (Outlook.MAPIFolder folder in mailFolders)
                    {
                        foreach (Outlook.MAPIFolder subFolder in folder.Folders)
                        {
                            if (subFolder.DefaultItemType == Outlook.OlItemType.olMailItem)
                            {
                                inboxSelections.Add(new InboxSelection() { IsSelected = true, Folder = subFolder.FullFolderPath ,EntryId=subFolder.EntryID});
                            }
                            subFolder?.ReleaseComObject();
                        }
                        folder?.ReleaseComObject();
                    }
                    bindingSource1.DataSource = this.inboxSelections;
                    mailFolders?.ReleaseComObject();
                    ns?.ReleaseComObject();

                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(string.Format("Outlook is not accessible, maybe it is running as administrator or have not finished loading ({0}).", ex.Message));
            }
        }
        CancellationTokenSource? cancellationTokenSource = new CancellationTokenSource();
        Dictionary<string, int> senderStatistics;
        private async void toolStripButtonStart_Click(object sender, EventArgs e)
        {
            // Cancel any previous operation
            cancellationTokenSource?.Cancel();
            cancellationTokenSource = new CancellationTokenSource();
            CancellationToken token = cancellationTokenSource.Token;
            senderStatistics = new Dictionary<string, int>();

            // Setup progress reporting
            var progress = new Progress<ProgressChangedEventArgs>(progressChangedEventArgs =>
            {
                // This code runs on the UI thread automatically
                this.toolStripProgressBar1.Value = progressChangedEventArgs.ProgressPercentage;
                this.toolStripStatusLabel1.Text = progressChangedEventArgs.UserState?.ToString() ?? "";
            });
            try
            {
                // Pass the token and progress reporter to the async task
                await CountMailBySenderTaskAsync(progress, token);
                this.toolStripProgressBar1.Value = 0;
                toolStripStatusLabel1.Text = "Operation Completed!";
            }
            catch (OperationCanceledException)
            {
                this.toolStripProgressBar1.Value = 0;
                toolStripStatusLabel1.Text = "Operation Cancelled.";
            }
            catch (System.Exception ex)
            {
                this.toolStripProgressBar1.Value = 0;
                toolStripStatusLabel1.Text = $"Error: {ex.Message}";
            }
            finally
            {
                // Re-enable buttons, clean up source
                cancellationTokenSource.Dispose();
                cancellationTokenSource = null;
            }
            if (senderStatistics?.Count > 0)
            {
                saveFileDialog1.FileName = "SenderStatistics.csv";
                if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    CsvHelper.Configuration.CsvConfiguration config = new CsvHelper.Configuration.CsvConfiguration(System.Globalization.CultureInfo.InvariantCulture)
                    {
                        Delimiter = ",",
                    };
                    using (var writer = new StreamWriter(saveFileDialog1.FileName))
                    using (var csv = new CsvHelper.CsvWriter(writer, config))
                    {
                        csv.WriteField("Sender");
                        csv.WriteField("MailCount");
                        csv.NextRecord();
                        foreach (var kvp in senderStatistics.OrderByDescending(kvp => kvp.Value))
                        {
                            csv.WriteField(kvp.Key);
                            csv.WriteField(kvp.Value);
                            csv.NextRecord();
                        }
                    }
                    Process.Start(new ProcessStartInfo()
                    {
                        FileName = saveFileDialog1.FileName,
                        UseShellExecute = true
                    } );
                }
            }
        }
        async Task CountMailBySenderTaskAsync(IProgress<ProgressChangedEventArgs> progress, CancellationToken token)
        {
            var ns = outlookApp.GetNamespace("MAPI");
            var mailFolders = ns.Folders;
            Dictionary<string, Outlook.MAPIFolder> mailFolderIndex = new Dictionary<string, Outlook.MAPIFolder>();

            foreach (Outlook.MAPIFolder mailFolder in mailFolders)
            {
                foreach (Outlook.MAPIFolder subFolder in mailFolder.Folders)
                {
                    if (subFolder.DefaultItemType == Outlook.OlItemType.olMailItem)
                    {
                        mailFolderIndex.Add(subFolder.EntryID, subFolder);
                    }
                }
                mailFolder?.ReleaseComObject();
            }
            var selectedFolders = inboxSelections.Where(i => i.IsSelected).ToList();
            if (selectedFolders.Count == 0) return;

            int totalFolders = selectedFolders.Count;
            int processedFolders = 0;
            foreach (var inboxSelection in selectedFolders)
            {
                if (cancellationTokenSource == null || cancellationTokenSource.Token.IsCancellationRequested)
                    break;
                var folder = mailFolderIndex[inboxSelection.EntryId];
                if (folder != null)
                {
                    var folderItems = folder.Items;
                    int mailCount = folderItems.Count;
                    int processedMails = 0;
                    int batchSize = 100;
                    int currentpositionInBatch = 0;

                    for (int i = 1; i <= mailCount; i++)
                    {
                        if (cancellationTokenSource == null || cancellationTokenSource.Token.IsCancellationRequested)
                            break;
                        try
                        {

                            var item = folderItems[i] as Outlook.MailItem;
                            if (item != null)
                            {
                                string sender = item.SenderEmailAddress;
                                if (sender != null)
                                {
                                    if (senderStatistics.ContainsKey(sender))
                                    {
                                        senderStatistics[sender]++;
                                    }
                                    else
                                    {
                                        senderStatistics[sender] = 1;
                                    }
                                    if (currentpositionInBatch >= batchSize)
                                    {
                                        int mailProgress = (int)(((processedFolders + processedMails / mailCount) / (double)totalFolders) * 100);
                                        progress.Report(new ProgressChangedEventArgs(mailProgress, $"Scanned folder: {inboxSelection.Folder}, processed mails: {processedMails}/{mailCount}"));
                                        await Task.Yield(); // Yield to keep UI responsive
                                        currentpositionInBatch = 0;
                                    }
                                }
                                item?.ReleaseComObject();
                            }
                        }
                        catch (System.Exception ex)
                        {
                            Debug.Write(string.Format("Error processing mail {0}: {1}", processedMails, ex.Message));
                            int mailProgress = (int)(((processedFolders + processedMails / mailCount) / (double)totalFolders) * 100);

                            progress.Report(new ProgressChangedEventArgs(mailProgress, $"Error Processing Mail: {ex.Message}, processed mails: {processedMails}/{mailCount}"));
                            await Task.Yield(); // Yield to keep UI responsive

                        }
                        processedMails++;
                        currentpositionInBatch++;
                    }
                    folderItems?.ReleaseComObject();
                    processedFolders++;
                    int progressPercentage = (int)((processedFolders / (double)totalFolders) * 100);
                    progress.Report(new ProgressChangedEventArgs(progressPercentage, $"Scanned folder: {inboxSelection.Folder}"));
                }
            }
            foreach (var item in mailFolderIndex.Values)
            {
                item?.ReleaseComObject();
            }
            mailFolders?.ReleaseComObject();
            ns?.ReleaseComObject();
        }

        private void toolStripButtonStop_Click(object sender, EventArgs e)
        {
            cancellationTokenSource?.Cancel();
        }

        private void FormMain_FormClosed(object sender, FormClosedEventArgs e)
        {

        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            cancellationTokenSource?.Cancel();
            if (outlookApp != null)
            {
                outlookApp.ReleaseComObject();
                outlookApp = null;
            }
        }
    }
}
