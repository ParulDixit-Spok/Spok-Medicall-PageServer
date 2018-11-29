@echo off

rem PageSrv.bat - 2011-10-20
rem 
rem Improved - Pageserver re-start batch file. 
rem  the application may be restarted, 
rem  or exited depending on the application exit code

rem note(s) To the installer:
rem
rem   Modify two lines of this batch file to accomodate your installation.
rem   these lines are identified by "to the installer." text.
rem
rem   1 - define the location of the location and name of the
rem       Batch-Start-Stop-Log file - BssLog.Txt file.
rem       recommended location is the location of other pageserver log files: 
rem       e.g. - c:\spok\pageserver\log\BSSlog.Txt
rem 
rem   2 - the application launch line at the batch file label :RUN below approximately line # 99.
rem 

rem To the Installer:
rem  This line defines the location and name of the batch-start-stop logfile.
rem 
set 	Logfile=c:\spok\pageserver\log\BSSlog.Txt


rem when pageserver exits, examine the application exit code
rem exit codes are:
rem 	errorlevel ==  0 - APP_EXIT_USER_REQUEST_SYSTEM
rem 	errorlevel ==  1 - APP_EXIT_USER_REQUEST_MENU
rem 	errorlevel ==  2 - APP_EXIT_INIT_APPPLICATION_FAILED
rem	errorlevel ==  3 - APP_EXIT_INIT_INSTANCE_FAILED               
rem	errorlevel ==  4 - APP_EXIT_BAD_COMMAND_LINE_OPTION                
rem	errorlevel ==  5 - APP_EXIT_ACCESS_EXCEPTION
rem                        -1073741819    C0000005
rem	errorlevel ==  6 - APP_EXIT_EXCEPTION_INT_DIVIDE_BY_ZERO    
rem                        -1073741676    C0000094        
rem	errorlevel ==  7 - APP_EXIT_PRIV_INSTRUCTION_EXCEPTION               
rem	errorlevel ==  8 - APP_EXIT_UNKNOWN_EXCEPTION                
rem	errorlevel ==  9 - APP_EXIT_NEW_EXE_DETECTED                
rem	errorlevel == 10 - APP_EXIT_UNEXPECTED_EXIT 
rem	errorlevel == 11 - APP_EXIT_SOFTWARE_EXCEPTION 
rem	errorlevel == 12 - APP_EXIT_BAD_PARAMETER_FILE
rem	errorlevel == 13 - APP_EXIT_EXIT_FOR_RESTART
rem	errorlevel == 14 - APP_EXIT_OPERATING_SYSTEM_VERSION_NG
rem	errorlevel == 15 - APP_EXIT_CANT_CREATE_LOG_SUBDIRECTORY


:run
cls
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .                         PageServer Re-Start Application.
	@echo .
	@echo .                       WARNING: DO NOT CLOSE THIS WINDOW !!!
	@echo .                                   (ok to minimize.)
	@echo .
	@echo .           (window closes automatically when the PageServer is closed.)
	@echo .

	rem Log a starup event, and run the PageServer.
	rem 
	date /T >DateFile.txt	
	for /f "tokens=2" %%I in (DateFile.txt) do set DATE_TEXT=%%I
	time /T >TimeFile.txt	
	for /f "tokens=1" %%I in (TimeFile.txt) do set TIME_TEXT=%%I
	@echo .                 PageServer Started on: %DATE_TEXT% at: %TIME_TEXT%.
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .
	@echo .
	@echo . >>%Logfile%
	@echo Starting Pageserver on: %DATE_TEXT% at: %TIME_TEXT%. >>%Logfile%

	rem to the Installer:
	rem  this line defines the application launch and options for the PageServer application.
	rem 
	c:\spok\pageserver\xapgsrv.exe -t

	rem capture the PageServer exit code.
	rem
 	set PsExitCode=%ERRORLEVEL%

	rem Log a shutdown event.
	rem 
	date /T >DateFile.txt	
	for /f "tokens=2" %%I in (DateFile.txt) do set DATE_TEXT=%%I
	time /T >TimeFile.txt	
	for /f "tokens=1" %%I in (TimeFile.txt) do set TIME_TEXT=%%I
	@echo Pageserver terminated with exit code: %PsExitCode% on: %DATE_TEXT% at: %TIME_TEXT%.
	@echo Pageserver terminated with exit code: %PsExitCode% on: %DATE_TEXT% at: %TIME_TEXT%. >>%Logfile%
	@echo . >>%Logfile%

	rem After the pageserver has exited, Examine exit code, and decide if re-start is indicated.
	
	rem Note that the number sequence is very important here.
	rem  and must go from high to low. 
	rem  negative numbers are lower than zero, 
	rem  and the larger a negative number is, the lower it is.
	
	if %PsExitCode% == 128 goto :run
	if %PsExitCode% == 16 goto :eof
	if %PsExitCode% == 15 goto :eof
	if %PsExitCode% == 14 goto :eof
	if %PsExitCode% == 13 goto :run
	if %PsExitCode% == 12 goto :eof
	if %PsExitCode% == 11 goto :eof
	if %PsExitCode% == 10 goto :eof
	if %PsExitCode% == 9  goto :eof
	if %PsExitCode% == 8  goto :eof
	if %PsExitCode% == 7  goto :eof
	if %PsExitCode% == 6  goto :run
	if %PsExitCode% == 5  goto :run
	if %PsExitCode% == 4  goto :eof
	if %PsExitCode% == 3  goto :eof
	if %PsExitCode% == 2  goto :eof
	if %PsExitCode% == 1  goto :eof
	if %PsExitCode% == 0  goto :eof
	if %PsExitCode% == -1073741676 goto :run
	if %PsExitCode% == -1073741819 goto :run

	goto :eof

