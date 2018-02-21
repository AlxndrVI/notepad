#include "notepad.h"
#include "ui_find.h"
#include "find.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication EditorApp(argc, argv);
    Notepad Editor;

    Editor.show();
    return EditorApp.exec();
}
