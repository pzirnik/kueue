#!/bin/bash 

tar cfvz kueue-$1.tar.gz --exclude="*.sh" --exclude=".kdev4" kueue-$1/
