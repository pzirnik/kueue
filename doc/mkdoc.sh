#!/bin/bash
#
# mkdoc.sh - Create HTML documentation for kueue
# This requires suse-xsl-stylesheets to be installed
# Available at Documentation:/Tools in the OBS
#

xsltproc --output index.html /usr/share/xml/docbook/stylesheet/suse/xhtml/chunk.xsl index.docbook
mkdir -p html/static/css
cp /usr/share/xml/docbook/stylesheet/suse/static/css/susebooks.css html/static/css

echo "" >> html/static/css/susebooks.css
echo "body:after {" >> html/static/css/susebooks.css
echo "content:" >> html/static/css/susebooks.css
for i in images/* ; do echo "url($i)" >> html/static/css/susebooks.css; done
echo ";" >> html/static/css/susebooks.css
echo "display: none;" >> html/static/css/susebooks.css
echo "}" >> html/static/css/susebooks.css

scp -r html/* root@kueue.hwlab.suse.de:/srv/www/htdocs/doc
scp -r images root@kueue.hwlab.suse.de:/srv/www/htdocs/doc
scp -r scripts root@kueue.hwlab.suse.de:/srv/www/htdocs/doc
