#include "math.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "simgrid/msg.h"            
#include <xbt/replay.h>
/* Create a log channel to have nice outputs. */
#include "xbt.h"
#include <stdlib.h>
#include "xbt/log.h"
#include "xbt/sysdep.h"
#include "xbt/asserts.h"
XBT_LOG_NEW_DEFAULT_CATEGORY(replay, "Messages specific for this example");
static xbt_dict_t opened_files = NULL;
int headsize;
#define ACT_DEBUG(...) \
  if (XBT_LOG_ISENABLED(replay, xbt_log_priority_verbose)) {  \
    char *NAME = xbt_str_join_array(action, " ");              \
    XBT_DEBUG(__VA_ARGS__);                                    \
    xbt_free(NAME);                                            \
  } else ((void)0)
//replace the str in the string to a target one
char *instr_process (const char *process_name, int process_pid, char *str, int len)
{
  snprintf (str, len, "%s-%d", process_name, process_pid);
  return str;
}
#define STR_SIZE 200
int len = STR_SIZE;
char str[STR_SIZE];
char *getpname(void)
{
  instr_process(MSG_process_get_name(MSG_process_self()), MSG_process_get_PID (MSG_process_self()), str, len);
  return str;
}
int ReplaceStr(char *sSrc,char *sMatchStr,char *sReplaceStr)
{
        int  StringLen;
        char caNewString[200];

        char *FindPos = strstr(sSrc, sMatchStr);
        if( (!FindPos) || (!sMatchStr) )
                return -1;

        while( FindPos )
        {
                memset(caNewString, 0, sizeof(caNewString));
                StringLen = FindPos - sSrc;
                strncpy(caNewString, sSrc, StringLen);
                strcat(caNewString, sReplaceStr);
                strcat(caNewString, FindPos + strlen(sMatchStr));
                strcpy(sSrc, caNewString);
                FindPos = strstr(sSrc, sMatchStr);
        }

        return 0;
}
static void log_action(const char *const *action, double date)
{
  if(XBT_LOG_ISENABLED(replay, xbt_log_priority_verbose)) {
    char *name = xbt_str_join_array(action, " ");
    XBT_VERB("%s %.9f", name, date);
    xbt_free(name);
  }
}

msg_file_t get_file_descriptor(const char *speed,const char *file_name,const char *index){//get file with the file full_name
  char full_name[1024];
  msg_file_t file = NULL;
  sprintf(full_name, "%s:%s:%s:%s", speed,file_name,MSG_process_get_name(MSG_process_self()),index);
  file = (msg_file_t) xbt_dict_get_or_null(opened_files, full_name);
 return file;
}

static sg_size_t parse_size(const char *string){
  sg_size_t size;
  char *endptr;
  size = strtoul(string, &endptr, 10);
  if (*endptr != '\0')
  {  THROWF(unknown_error, 0, "%s is not a long unsigned int (a.k.a. sg_size_t)", string);
  }  
  return size;
}
static sg_offset_t parse_offset(const char *string){
  sg_offset_t offset;
  offset = atol(string);
  return offset;
}
int p=0;
int i=0;
void simsleep(const char *const *action) {//the sleep function that creats and executes the task which only sleep
  i++;  
  const char *processid = action[0];
  const char *worker = action[1];
  const char *worktime = action[3];
  double sleeptime;
  double clock = MSG_get_clock();       /* this "call" is free thanks to inlining */
  sleeptime = atof(worktime); 
  msg_task_t task=NULL;
  task = MSG_task_create("simsleep",sleeptime*MSG_get_host_speed(MSG_host_self()),0,NULL);
// task = MSG_task_create("simsleep",sleeptime,0,NULL);
  char simworker[80]="sim";
  strcat(simworker,worker);
 if(strcmp(simworker,"simopen")==0||strcmp(simworker,"simcreat")==0||strcmp(simworker,"simrelease")==0||strcmp(simworker,"simread")==0||strcmp(simworker,"simwrite")==0)
 {
  MSG_task_set_category(task,simworker);
   XBT_INFO("set category simworker %s",simworker);
  TRACE_host_push_state(getpname(),"MyState",simworker);
  MSG_task_execute(task);
  MSG_task_destroy(task);
  TRACE_host_pop_state(getpname(),"MyState");
 }
else
 {
//  MSG_task_set_category(task,"simsleep");
 // XBT_INFO("set category simworker %s","simsleep");
  TRACE_host_push_state(getpname(),"MyState","simsleep");
  MSG_task_execute(task);
  MSG_task_destroy(task);
  TRACE_host_pop_state(getpname(),"MyState");
 }
 // MSG_process_sleep(sleeptime); 
  log_action(action, MSG_get_clock() - clock);
  XBT_INFO("%s  worker %s is done,time is %s ",worker,processid,worktime);
}

static void simopen(const char *const *action) {
  const char *processid = action[0];
  const char *worktime = action[3];
  double sleeptime;
  const char *index = action[4];
  char full_name[1024];
  msg_file_t file = NULL;
  double clock = MSG_get_clock();       /* this "call" is free thanks to inlining */
  //Because the action open is too fast ,to catch up with the clock of the simulator, we let it to sleep for a while.
  sleeptime = atof(worktime);
  //MSG_process_sleep(sleeptime);
  //open slow filename
  const char *file_name = action[2];
  msg_task_t task=NULL;
  task = MSG_task_create("simopen",sleeptime*MSG_get_host_speed(MSG_host_self()),0,NULL);
  MSG_task_set_category(task,"simopen");
  TRACE_host_push_state(getpname(),"MyState","simopen");
  MSG_task_execute(task);
  MSG_task_destroy(task);
  TRACE_host_pop_state(getpname(),"MyState");
  char fn[200];
   strcpy(fn,file_name);
  ReplaceStr(fn,"/home","/slow");
  sprintf(full_name, "%s:%s:%s:%s","slow",file_name,MSG_process_get_name(MSG_process_self()),index);
  ACT_DEBUG("Entering Open: %s (filename: %s)", NAME, file_name);
  file = MSG_file_open(fn, NULL);
  xbt_dict_set(opened_files, full_name, file, NULL);
  //open fast filename
   strcpy(fn,file_name);
  ReplaceStr(fn,"/home","/fast");
  sprintf(full_name, "%s:%s:%s:%s","fast",file_name,MSG_process_get_name(MSG_process_self()),index);
  ACT_DEBUG("Entering Open: %s (filename: %s)", NAME, file_name);
  file = MSG_file_open(fn, NULL);
  xbt_dict_set(opened_files, full_name, file, NULL);
 
  log_action(action, MSG_get_clock() - clock);
  XBT_INFO("open worker %s%s is done",processid,index);
}
static void simrelease(const char *const *action) {
  const char *processid = action[0];
  const char *file_name = action[2];
  const char *worktime = action[3];
  double sleeptime;
  const char *index = action[4];
  msg_file_t file;
  double clock = MSG_get_clock();       /* this "call" is free thanks to inlining */
  //Because the action release is too fast ,to catch up with the clock of the simulator, we let it to sleep for a while.
  sleeptime = atof(worktime);
  //MSG_process_sleep(sleeptime);
  msg_task_t task=NULL;
  task = MSG_task_create("simrelease",sleeptime*MSG_get_host_speed(MSG_host_self()),0,NULL);
  MSG_task_set_category(task,"simrelease");
  TRACE_host_push_state(getpname(),"MyState","simrelease");
  MSG_task_execute(task);
  MSG_task_destroy(task);
  TRACE_host_pop_state(getpname(),"MyState");
  //close slow file
  file = get_file_descriptor("slow",file_name,index);
  ACT_DEBUG("Entering Close: %s (filename: %s)", NAME, file_name);
  MSG_file_close(file);
  //close fast file
  file = get_file_descriptor("fast",file_name,index);
  ACT_DEBUG("Entering Close: %s (filename: %s)", NAME, file_name);
  MSG_file_close(file);
 
  log_action(action, MSG_get_clock() - clock);
  XBT_INFO("release worker %s%s is done",processid,index);
}

static void simread(const char *const *action) {
  const char *processid = action[0];
  const char *file_name = action[2];
  const char *worktime = action[3];
  double sleeptime;
  sleeptime = atof(worktime);
  const char *index = action[4];
  const char *position_str =action[5];
  const char *size_str = action[6];
  msg_file_t file = NULL;
  sg_size_t size = parse_size(size_str);
  sg_offset_t position= parse_offset(position_str);
  msg_task_t task=NULL;
  task = MSG_task_create("simread",sleeptime*MSG_get_host_speed(MSG_host_self()),0,NULL);
  MSG_task_set_category(task,"simread");
  TRACE_host_push_state(getpname(),"MyState","simread");
  MSG_task_execute(task);
  MSG_task_destroy(task);
  TRACE_host_pop_state(getpname(),"MyState");
  double clock = MSG_get_clock();       /* this "call" is free thanks to inlining */
  
  if(position<headsize && (position+size)<=headsize)
  {
  file = get_file_descriptor("fast",file_name,index);
  ACT_DEBUG("Entering Read: %s (size: %llu)", NAME, size);
  MSG_file_seek(file,position,SEEK_SET);
  MSG_file_read(file, size);
  XBT_INFO("1 position is  %llu ,size is %llu",position,size);
 
  }
  else if(position<headsize && (position+size)>headsize)
  {
  //read the Head part
  file = get_file_descriptor("fast",file_name,index);
  ACT_DEBUG("Entering Read: %s (size: %llu)", NAME, size);
  MSG_file_seek(file,position,SEEK_SET);
  MSG_file_read(file, (headsize-position));
  //read the data part
  file = get_file_descriptor("slow",file_name,index);
  ACT_DEBUG("Entering Read: %s (size: %llu)", NAME, size);
  MSG_file_seek(file,headsize,SEEK_SET);
  MSG_file_read(file, size-(headsize-position));
  XBT_INFO("2 position is  %llu ,size is %llu",position,size);
  }
  else
  {
  XBT_INFO("3 position is  %llu ,size is %llu",position,size);
  file = get_file_descriptor("slow",file_name,index);
  ACT_DEBUG("Entering Read: %s (size: %llu)", NAME, size);
  MSG_file_seek(file,position,SEEK_SET);
  MSG_file_read(file, size);
  }
  
  log_action(action, MSG_get_clock() - clock);
  XBT_INFO("read  worker %s%s is done",processid,index);
}
static void simcreat(const char *const *action) {
  const char *processid = action[0];
  const char *worktime = action[3];
  double sleeptime;
  const char *index = action[4];
  char full_name[1024];
  msg_file_t file = NULL;
  double clock = MSG_get_clock();       /* this "call" is free thanks to inlining */
  sleeptime = atof(worktime);
  //MSG_process_sleep(sleeptime);
  
  msg_task_t task=NULL;
  task = MSG_task_create("simcreat",sleeptime*MSG_get_host_speed(MSG_host_self()),0,NULL);
  MSG_task_set_category(task,"simcreat");
  TRACE_host_push_state(getpname(),"MyState","simcreat");
  MSG_task_execute(task);
  MSG_task_destroy(task);
  TRACE_host_pop_state(getpname(),"MyState");
  //open slow file
  const char *file_name = action[2];
  char fn[200];
   strcpy(fn,file_name);
  ReplaceStr(fn,"/home","/slow");
  sprintf(full_name, "%s:%s:%s:%s","slow",file_name,MSG_process_get_name(MSG_process_self()), index);
  ACT_DEBUG("Entering Creat: %s (filename: %s)", NAME, file_name);
  file = MSG_file_open(fn, NULL);
  xbt_dict_set(opened_files, full_name, file, NULL);
  //open fast version of file
  strcpy(fn,file_name);
  ReplaceStr(fn,"/home","/fast");
  sprintf(full_name, "%s:%s:%s:%s","fast",file_name,MSG_process_get_name(MSG_process_self()), index);
  ACT_DEBUG("Entering Creat: %s (filename: %s)", NAME, file_name);
  file = MSG_file_open(fn, NULL);
  xbt_dict_set(opened_files, full_name, file, NULL);
  
  log_action(action, MSG_get_clock() - clock);
  XBT_INFO("creat worker %s %s is done",processid,index);
}
static void simwrite(const char *const *action) {
  const char *processid = action[0];
  const char *file_name = action[2];
  const char *worktime = action[3];
  const char *index = action[4];
  const char *position_str =action[5];
  const char *size_str = action[6];
  msg_file_t file = NULL;
  sg_size_t size = parse_size(size_str);
  sg_offset_t position= parse_offset(position_str);
  double clock = MSG_get_clock();       /* this "call" is free thanks to inlining */
  msg_task_t task=NULL;
  double sleeptime;
  sleeptime = atof(worktime);
  task = MSG_task_create("simwrite",sleeptime*MSG_get_host_speed(MSG_host_self()),0,NULL);
  MSG_task_set_category(task,"simwrite");
  TRACE_host_push_state(getpname(),"MyState","simwrite");
  MSG_task_execute(task);
  MSG_task_destroy(task);
  TRACE_host_pop_state(getpname(),"MyState");
  file = get_file_descriptor("slow",file_name,index);
  ACT_DEBUG("Entering Read: %s (size: %llu)", NAME, size);
  MSG_file_seek(file,position,SEEK_SET);
  MSG_file_write(file, size);
  XBT_INFO("20 position is  %llu ,size is %llu",position,size);
  XBT_INFO("write worker %s%s is done",processid,index);
  log_action(action, MSG_get_clock() - clock);
}
int main(int argc, char *argv[]) {
  /* Check the given arguments */
  MSG_init(&argc, argv);
  /* Explicit initialization of the action module is required now*/
  MSG_action_init();
 // MSG_config("surf/precision","1e-9");
  MSG_create_environment(argv[1]);
 /*  Simulation setting */
  msg_error_t res = MSG_OK;
  TRACE_category_with_color ("simsleep", "1 1 1"); //white
  TRACE_category_with_color ("simopen", "0 0 0");    //black
  TRACE_category_with_color ("simrelease", "0 0 0");//black
  TRACE_category_with_color ("simread", "1 0 0"); //red
  TRACE_category_with_color ("simcreat", "0 0 0");//black
  TRACE_category_with_color ("simwrite", "0 0 1");//blue
  instr_new_user_state_type("MSG_PROCESS", "MyState"); 
  TRACE_host_state_declare_value ("MyState", "simopen", "0 0 0");    //black
  TRACE_host_state_declare_value ("MyState", "simcompute", "0 1 0");    //green
  TRACE_host_state_declare_value ("MyState", "simrelease", "0 0 0");//black
  TRACE_host_state_declare_value ("MyState", "simread", "1 0 0"); //red
  TRACE_host_state_declare_value ("MyState", "simsleep", "1 1 1");    //white
  TRACE_host_state_declare_value ("MyState", "simcreat", "0 0 0");   //black
  TRACE_host_state_declare_value ("MyState", "simwrite", "0 0 1");//blue
/* No need to register functions as in classical MSG programs: the actions get started anyway */
  MSG_launch_application(argv[2]);
  char *wh=argv[4];
  XBT_INFO("Your choose is %s",wh);
  headsize=atoi(argv[5]);
  xbt_replay_action_register("unlink", simsleep);//register the action
  xbt_replay_action_register("compute", simsleep);//register the action
  xbt_replay_action_register("access", simsleep);
  xbt_replay_action_register("stat", simsleep);
  xbt_replay_action_register("flush", simsleep);
  xbt_replay_action_register("readdir", simsleep);
  xbt_replay_action_register("getxattr", simsleep);
  xbt_replay_action_register("mkdir", simsleep);
  xbt_replay_action_register("symlink", simsleep);
  xbt_replay_action_register("readlink", simsleep);
  if(strcmp(wh,"action")==0) //decide which mode to run
  /*   Action registration */
 {
  xbt_replay_action_register("open", simopen);
  xbt_replay_action_register("release", simrelease);
  xbt_replay_action_register("read", simread);
  xbt_replay_action_register("creat", simcreat);
  xbt_replay_action_register("write", simwrite);
 }
  else if(strcmp(wh,"sleep")==0){
  xbt_replay_action_register("open", simsleep);
  xbt_replay_action_register("release", simsleep);
  xbt_replay_action_register("read", simsleep);
  xbt_replay_action_register("creat", simsleep);
  xbt_replay_action_register("write", simsleep);
  }
 
  if(!opened_files)
  {opened_files = xbt_dict_new_homogeneous(NULL);}
  res = MSG_action_trace_run(argv[3]);  // it's ok to pass a NULL argument here
  
  XBT_INFO("Simulation time %g", MSG_get_clock());
  if(opened_files)
	{xbt_dict_free(&opened_files);}
 MSG_action_exit();
  return !!MSG_OK;
}
