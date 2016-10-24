using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SelenuimTest
{
    class Program
    {
        static void Main(string[] args)
        {            
            //Install Google Chrome first before running this
            using (IWebDriver driver = new ChromeDriver())
            {
                driver.Manage().Timeouts().ImplicitlyWait(TimeSpan.FromSeconds(10));//wait 10 seconds before polling DOM
                //driver.Url = "http://www.wikipedia.org";
                //IWebElement searchInput= driver.FindElement(By.Id("searchInput"));
                //searchInput.SendKeys("Main Page");
                //IWebElement searchForm= driver.FindElement(By.Id("search-form"));
                //searchForm.Submit();
                ////find the span for 'From today's featured article'
                //IWebElement todayFeatureArticleBanner = driver.FindElement(By.Id("From_today.27s_featured_article"));
                ////then go up to find the tr element containing the span
                //IWebElement todayFeatureArticleBannerRow = todayFeatureArticleBanner.FindElement(By.XPath("../../.."));                
                ////then find its first tr slibing
                //IWebElement todayFeatureArticleRow = todayFeatureArticleBannerRow.FindElement(By.XPath("following-sibling::tr[1]"));
                ////then find the first bolded italic link at any level, // is for looping through all children
                //IWebElement todayFeatureArticleLink = todayFeatureArticleRow.FindElement(By.XPath("//i/b/a"));
                //Debug.WriteLine(String.Format("Today's Featured Article is \"{0}\", which is listed under the following categories:", todayFeatureArticleLink.Text));
                //todayFeatureArticleLink.Click();
                ////find an element with id catlinks
                //var categoryLinksDiv = driver.FindElement(By.Id("catlinks"));
                ////find all elements under the element with a div/ul/li path 
                //var categoryLinksLis = categoryLinksDiv.FindElements(By.XPath("div/ul/li"));
                //foreach (var category in categoryLinksLis)
                //{
                //    Debug.WriteLine(category.Text);
                //}
                driver.Url = "http://www.w3schools.com/xml/tryit.asp?filename=tryajax_first";
                IWebElement rightFrame= driver.FindElement(By.Id("iframeResult"));
                driver.SwitchTo().Frame(rightFrame);
                IWebElement demo = driver.FindElement(By.Id("demo"));
                Debug.WriteLine("Before:");
                Debug.WriteLine(demo.Text);
                IWebElement changeContentButton = demo.FindElement(By.TagName("button"));
                changeContentButton.Click();

                demo = driver.FindElement(By.Id("demo"));
                var ajaxResult= demo.FindElement(By.TagName("p"));
                Debug.WriteLine("After:");
                Debug.WriteLine(demo.Text);
                driver.SwitchTo().DefaultContent();
            }
        }
    }
}
