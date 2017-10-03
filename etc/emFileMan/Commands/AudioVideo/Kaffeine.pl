#!/usr/bin/perl
#[[BEGIN PROPERTIES]]
# Type = Command
# Order = 1.0
# Interpreter = perl
# DefaultFor = .aac:.asf:.au:.avi:.flac:.fli:.flv:.it:.m2t:.m2v:.mkv:.mod:.mov:.mp3:.mp4:.mpeg:.mpg:.mpv:.ogg:.ogv:.ra:.rm:.s3m:.stm:.wav:.vob:.wma:.wmv:.xm
# Caption = Kaffeine
# Descr =Open audio or video files in the Kaffeine Player. It is
# Descr =even possible to open a directory which contains files to
# Descr =be played.
# Descr =
# Descr =Selection details:
# Descr =
# Descr =  Source: Ignored.
# Descr =
# Descr =  Target: The files or the directory to be opened.
# Icon = thirdparty/kaffeine.tga
#[[END PROPERTIES]]

use strict;
use warnings;
BEGIN { require "$ENV{'EM_DIR'}/res/emFileMan/scripts/cmd-util.pl"; }

OpenTargetsWith('kaffeine');
