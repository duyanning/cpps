#include "config.h"
#include "UpdateGraphAction.h"
#include "CompilerInfo.h"

UpdateGraphAction::UpdateGraphAction(FileEntityPtr obj)
    :
    m_obj(obj)
{
}

UpdateGraphActionPtr makeUpdateGraphAction(FileEntityPtr obj)
{
    return UpdateGraphActionPtr(new UpdateGraphAction(obj));
}


void split_line_to_items(string line, vector<string>& items)
{
    if (cc == CC::VC) {
        items.push_back(line);
    }
    else {
        regex item_pat{ R"(([^ \\]|\\ |\\)+)" };
        // 这个正则表达式可以从如下文本中匹配出空格分隔的项，但同时把“\ ”视为路径的一部分。
        /*
C:\Users\duyanning\.cpps\cache\myprj\cpps\test\hello-pch\main.cpp.o:
F:\myprj\cpps\test\hello-pch\main.cpp F:\myprj\cpps\test\hello-pch\std.h
C:\Users\duyanning\Desktop\New\ folder\main.cpp f:/a_c你好/b.h
        */

        //cout << "line: " << line << endl;
        smatch matches;
        while (regex_search(line, matches, item_pat)) {
            //cout << "     " << matches.str() << endl;
            items.push_back(matches.str());
            line = matches.suffix();
        }
    }
}

bool UpdateGraphAction::execute(const DepInfo& info)
{
    FileEntityPtr dep = static_pointer_cast<FileEntity>(info.all[0]);
    fs::path dep_path = dep->path();

    ifstream f(dep_path.string());


/*
读取.d文件的内容。.d文件的例子如下：

C:\Users\duyanning\.cpps\cache\myprj\cpps\test\multiple-srcs\foo.cpp.o: \
 F:\myprj\cpps\test\multiple-srcs\foo.cpp \
 F:\myprj\cpps\test\multiple-srcs\foo.h
*/

    // 下面出现的“到此为止”是为了避免注释中的斜杠引发编译器警告
    // .d文件中路径中的空格的处理是个问题。
    // mingw会在空格前面加上\   到此为止
    // 如  C:\Users\duyanning\Desktop\New\ folder\main.cpp
    // vc不会加
    // 如   C:\Users\duyanning\Desktop\New folder\main.cpp \   到此为止


    // mingw还有把多个文件放在一行用空格隔开的情况，而且就在第二行，所以第二行还不能跳过，如：
 /*
 C:\Users\duyanning\.cpps\cache\myprj\cpps\test\hello-pch\main.cpp.o: \
 F:\myprj\cpps\test\hello-pch\main.cpp F:\myprj\cpps\test\hello-pch\std.h \
 F:\myprj\cpps\test\hello-pch\foo.h

 */

    // 跳过的应该是前两个，而不是前两行。一行可能包含多个。
    // vc不会一行多个
    // mingw会一行多个，gcc或许也会一行多个。 字母后的空格表示分开多个。反斜杠后的空格，表示路径中的空格。

    string line;
    int count = 0;
    while (getline(f, line)) {


        if (line.back() == '\\')
            line.pop_back();

        boost::algorithm::trim(line);

        if (line.empty())
            return true;
        // 如果把一个空串交给boost_1_76_0的split_line_to_items，得到items里有一个元素。
        // 上面return就是为了避免这点。

        vector<string> items;
        split_line_to_items(line, items);

        for (auto item : items) {
            count++;
            // 跳过前两项
            if (count > 2) {
                //cout << item << endl;
                fs::path p(item);
                m_obj->addPrerequisite(makeFileEntity(p));
            }
        }



    }

    /*
    istream_iterator<string> i = istream_iterator<string>(f);

    int count = 0;
    while (count < 2) {
        if (*i != "\\")
            count++;
        ++i;
    }

    for (; i != istream_iterator<string>(); ++i) {
        string t = *i;
        if (t != "\\") {
            fs::path p(t);
            m_obj->addPrerequisite(makeFileEntity(p));
        }
    }
    */
    return true;
}

