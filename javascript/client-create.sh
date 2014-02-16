browserify client.js > client.browser.js
uglifyjs -m -v client.browser.js > client.min.js
browserify client.noloc.src.js > client.noloc.js
uglifyjs -m -v client.noloc.js > client.noloc.min.js