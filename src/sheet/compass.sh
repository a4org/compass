#! /bin/bash


# 0. change dir

cd /var/www/compass/src/sheet

# 1. PrelimCheck sheet

# 1.1 download today's data
node prelimcheck/download.js

# 1.2 parse them
./seqparser

# 1.3 upload today's data
node prelimcheck/upload.js

# 1.4 get today's line, which is tomorrow's start index
node prelimcheck/checklines.js


# 2. HistPrices sheet

# 2.1 download today's data
node histprices/histdown.js

# 2.2 parse them
./histparser

# 2.3 upload today's data
node histprices/histup.js

# 2.4 get today's row, which is tomorrow's start index
node histprices/row.js
