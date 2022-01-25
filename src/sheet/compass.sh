#! /bin/bash

# 1. download today's data
node download.js

# 2. parse them
./parse

# 3. upload today's data
node upload.js

# 4. get today's line, which is tomorrow's start index
node checklines.js
