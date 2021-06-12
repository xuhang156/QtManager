#include "AbstractReportFile.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include "RrSus018DReportFile.h"
#include "util_json.h"

const QString xlsxTemplatePath = QStringLiteral("D:");
bool AbstractReportFile::save(QString filePath, bool releaseBook)
{
    if (book.isNull())
        return false;
    book->save(filePath.toStdWString().c_str());
    if (releaseBook)
        book->release();
#ifdef _WIN32
    ::ShellExecuteA(NULL, "open", filePath.toStdString().c_str(), NULL, NULL, SW_SHOW);
#elif __APPLE__
#endif
}

bool AbstractReportFile::init(QString productAndModelName)
{
    QString xlsxIndexJson =
        QString("%1/%2_xlsx_index.json").arg(xlsxTemplatePath, productAndModelName);
    return setFromJson(mmind::loadJson(xlsxIndexJson).object());
}

AbstractReportFile* createReportFileObj(QString productAndModelName)
{
    if (productAndModelName == "FrSus_018D") {
        auto reportObj = new RrSus018DReportFile();
        if (reportObj->init(productAndModelName))
            return reportObj;
        delete reportObj;
    }
    return nullptr;
}
