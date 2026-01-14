[ui]
scale=1
font_path=/usr/local/share/fonts/SourceCodePro-Regular.ttf
font_size_interface=10
font_size_code=10
restore_watch_window=1
layout=h(75,v(75,Source,Console),v(50,t(Commands,Locals,Watch,Struct,Exe),t(Stack,Thread,Breakpoints,Files,Data,CmdSearch))))

[executable]
path=out
arguments=
ask_directory=0

[gdb]
arguments=-ex "file out"

[commands]
START PROGRAM=file out;
SAVE BREAKPOINTS=save breakpoints .breakpoints;
LOAD BREAKPOINTS=source .breakpoints;
