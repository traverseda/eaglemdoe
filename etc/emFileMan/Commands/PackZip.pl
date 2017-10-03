#!/usr/bin/perl
#[[BEGIN PROPERTIES]]
# Type = Command
# Order = 15.0
# Interpreter = perl
# Caption = Pack zip
# Descr =Create a ZIP archive from files and directories.
# Descr =The name of the archive file is asked.
# Descr =
# Descr =Selection details:
# Descr =
# Descr =  Source: The files and directories to be packed.
# Descr =
# Descr =  Target: The directory in which the archive file
# Descr =          shall be created.
# Icon = pack_file_zip.tga
# Hotkey = Meta+P
#[[END PROPERTIES]]

use strict;
use warnings;
BEGIN { require "$ENV{'EM_DIR'}/res/emFileMan/scripts/cmd-util.pl"; }

PackType('zip');
