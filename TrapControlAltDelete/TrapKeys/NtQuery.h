#ifdef __cplusplus
extern "C" {
#endif

typedef LONG NTSTATUS;
typedef struct _PEB *PPEB;
typedef ULONG KAFFINITY;
typedef KAFFINITY *PKAFFINITY;
typedef LONG KPRIORITY;


typedef enum _PROCESSINFOCLASS 
{
   ProcessBasicInformation,
   ProcessQuotaLimits,
   ProcessIoCounters,
   ProcessVmCounters,
   ProcessTimes,
   ProcessBasePriority,
   ProcessRaisePriority,
   ProcessDebugPort,
   ProcessExceptionPort,
   ProcessAccessToken,
   ProcessLdtInformation,
   ProcessLdtSize,
   ProcessDefaultHardErrorMode,
   ProcessIoPortHandlers,          // Note: this is kernel mode only
   ProcessPooledUsageAndLimits,
   ProcessWorkingSetWatch,
   ProcessUserModeIOPL,
   ProcessEnableAlignmentFaultFixup,
   ProcessPriorityClass,
   ProcessWx86Information,
   ProcessHandleCount,
   ProcessAffinityMask,
   ProcessPriorityBoost,
   MaxProcessInfoClass
} PROCESSINFOCLASS;


typedef struct _PROCESS_BASIC_INFORMATION 
{
   NTSTATUS    ExitStatus;
   PPEB        PebBaseAddress;
   KAFFINITY   AffinityMask;
   KPRIORITY   BasePriority;
   ULONG       UniqueProcessId;
   ULONG       InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;


NTSYSAPI NTSTATUS NTAPI NtQueryInformationProcess(
   IN HANDLE ProcessHandle,
   IN PROCESSINFOCLASS ProcessInformationClass,
   OUT PVOID ProcessInformation,
   IN ULONG ProcessInformationLength,
   OUT PULONG ReturnLength OPTIONAL
   );


#ifdef __cplusplus
}
#endif
