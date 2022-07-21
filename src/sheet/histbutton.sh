# 2. HistPrices sheet

node histprices/row.js

# 2.1 download today's data
node histprices/histdown.js

# 2.2 parse them
./histparser

# 2.3 upload today's data
node histprices/histup.js

# 2.4 get today's row, which is tomorrow's start index
node histprices/row.js
