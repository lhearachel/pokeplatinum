#!/usr/bin/env bash

##  make-compile-commands.sh - create compile_commands.json from a `Makefile`
##  Copyright (C) 2025 - <lhearachel@proton.me>
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program.  If not, see <https://www.gnu.org/licenses/>.

##  Usage: ./make-compile-commands.sh
##
##  Run this script in the same directory as your `Makefile`. The generated
##  `compile_commands.json` will be written to standard output.

make --always-make --dry-run \
    | grep -wE "^cc" \
    | grep -w "\-c" \
    | jq -nR '[ inputs | {
        directory: "'"$(pwd)"'",
        command: .,
        file: match(" [^ ]+$").string[1:]
    } ]'
