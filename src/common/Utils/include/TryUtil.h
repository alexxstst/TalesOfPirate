#pragma once

#ifndef __CATCH_STACK
#define __CATCH_STACK

#undef T_B
#undef T_E

#define __CATCH

#ifdef __CATCH
	
    #define CATCH_EXCEPTION

    #pragma warning(push, 3) 
      #include <exception>
    #pragma warning(pop) 


#endif

#if defined( CATCH_EXCEPTION )
    #define T_FINAL }catch(std::exception &e) \
				{ \
					std::cout << e.what() << std::endl;\
					\
					std::string	strfile;\
					LG_GetDir(strfile);\
					strfile += "\\exception.txt";\
					FILE * fp = fopen(strfile.c_str(),"a+");\
					\
					if(fp) \
					{ \
						SYSTEMTIME st;\
						char tim[100] = {0};\
						GetLocalTime(&st);\
						sprintf(tim, "%02d-%02d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour,\
								st.wMinute, st.wSecond);\
						\
						fwrite(tim,strlen(tim),1,fp);\
						fwrite(__FUNCTION__,sizeof(__FUNCTION__) - 1,1,fp); \
						fwrite(e.what(),strlen(e.what()) - 1,1,fp); \
						fclose(fp); \
					} \
				}
    #define T_REPORT } catch(std::exception &e) \
				{ \
					std::string	strfile;\
					LG_GetDir(strfile);\
					strfile += "\\exception.txt";\
					FILE * fp = fopen(strfile.c_str(),"a+");\
					\
					if(fp) \
					{ \
						SYSTEMTIME st;\
						char tim[100] = {0};\
						GetLocalTime(&st);\
						sprintf(tim, "%02d-%02d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour,\
								st.wMinute, st.wSecond);\
						\
						fwrite(tim,strlen(tim),1,fp);\
						fwrite(__FUNCTION__,sizeof(__FUNCTION__) - 1,1,fp); \
						fwrite(e.what(),strlen(e.what()) - 1,1,fp); \
						fclose(fp); \
					} \
					WinExec("system/errorreport.exe", SW_SHOW); \
				}
    #define T_B try { 
	
    #define T_E	} \
				catch(std::exception &e) \
				{ \
					std::string	strfile;\
					LG_GetDir(strfile);\
					strfile += "\\exception.txt";\
					FILE * fp = fopen(strfile.c_str(),"a+");\
					\
					if(fp) \
					{ \
						SYSTEMTIME st;\
						char tim[100] = {0};\
						GetLocalTime(&st);\
						sprintf(tim, "%02d-%02d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour,\
								st.wMinute, st.wSecond);\
						\
						fwrite(tim,strlen(tim),1,fp);\
						fwrite(__FUNCTION__,sizeof(__FUNCTION__) - 1,1,fp); \
						fwrite(e.what(),strlen(e.what()) - 1,1,fp); \
						fclose(fp); \
					} \
				     throw; \
                }
    #define T_EXIT	} \
				catch(std::exception &e) \
				{ \
				try {\
					std::string	strfile;\
					LG_GetDir(strfile);\
					strfile += "\\exception.txt";\
					FILE * fp = fopen(strfile.c_str(),"a+");\
					\
					if(fp) \
					{ \
						SYSTEMTIME st;\
						char tim[100] = {0};\
						GetLocalTime(&st);\
						sprintf(tim, "%02d-%02d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour,\
								st.wMinute, st.wSecond);\
						\
						fwrite(tim,strlen(tim),1,fp);\
						fwrite(__FUNCTION__,sizeof(__FUNCTION__) - 1,1,fp); \
						fwrite(e.what(),strlen(e.what()) - 1,1,fp); \
						fclose(fp); \
					} \
					 ChaException(g_ulCurID, g_lCurHandle); \
				}\
				catch(...)\
				{}\
                }
	#define TRY_END_INFO(str) } \
				catch(...) \
				{ \
					std::string	strfile;\
					LG_GetDir(strfile);\
					strfile += "\\exception.txt";\
					FILE * fp = fopen(strfile.c_str(),"a+");\
					\
					if(fp) \
					{ \
						SYSTEMTIME st;\
						char tim[100] = {0};\
						GetLocalTime(&st);\
						sprintf(tim, "%02d-%02d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour,\
								st.wMinute, st.wSecond);\
						\
						fwrite(tim,strlen(tim),1,fp);\
						fwrite(__FUNCTION__,sizeof(__FUNCTION__) - 1,1,fp); \
						fwrite(e.what(),strlen(e.what()) - 1,1,fp); \
						fclose(fp); \
					} \
					throw; \
				}
	#define TRY_END_NOTHROW } \
				catch(...) \
				{ \
				}
	#define TRY_END_RETURN(str) } \
				catch(...) \
				{ \
					return str; \
				}

#else

	#define T_B
	#define T_E
    #define T_FINAL
	#define TRY_END_INFO(str)
	#define TRY_END_NOTHROW
	#define TRY_END_RETURN(str)

#endif

#endif
