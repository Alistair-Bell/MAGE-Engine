echo "Cleaning build files"
find -type d -name Build -a -prune -exec rm -rf {} \;
find . -name "Makefile" -delete
