#ifndef LIGHTWEIGHT_KEYDISPATCHANR_H
#define LIGHTWEIGHT_KEYDISPATCHANR_H


namespace android {
	class LightWeight_KeyDispatchAnr{
	public:
		static pthread_mutex_t mut_keydispatch_anr_start;
		static pthread_cond_t  cond_keydispatch_anr_start;

		static pthread_mutex_t mut_keydispatch_anr_timeout;
		static pthread_cond_t  cond_keydispatch_anr_timeout;
		
		static int curPid;
		static bool IS_ENG_BUILD;
	public:
		LightWeight_KeyDispatchAnr();
		~LightWeight_KeyDispatchAnr();
		static bool createMonitorThread();
		static void* monitorThread(void*);
		static void createTracesfiles(int pid);
        static char * nexttoksep(char **strp, char *sep);
		static char * nexttok(char **strp);
 		static int parse_pid(int *dump_pid_array);
        static int ps_parser(int pid,  char *proc_name, int ppid_flag);
		static int dump_java_backtrace();
		
	};
	
}

#endif
