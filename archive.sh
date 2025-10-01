TARGET="TRA0163_ZPG_Projekt.zip"

zip -r $TARGET \
                ./ZPGine/src/ \
                ./ZPGine/CMakeLists.txt \
                ./ZPGine/ZPGine.h \
                ./Client/ \
                CMakeLists.txt \
                assets/ \
                photos/
