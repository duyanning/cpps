#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

//- vc-linklib kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32
//- vc-linklib F:\vcpkg\installed\x86-windows\lib\*.lib
// 上面这两行写在这里太麻烦，可以在config.txt中使用lib来代替。我在上两行的//后加上-，可以避免cpps将其视为指令

// vc-extra-compile-flags: /MD /source-charset:utf-8 

int main()
{
	using namespace nana;

	//Define a form.
	form fm;

	//Define a label and display a text.
	label lab{ fm, "Hello, <bold blue size=16>Nana C++ Library</>" };
	lab.format(true);

	//Define a button and answer the click event.
	button btn{ fm, "Quit" };
	btn.events().click([&fm] {
		fm.close();
		});

	//Layout management
	fm.div("vert <><<><weight=80% text><>><><weight=24<><button><>><>");
	fm["text"] << lab;
	fm["button"] << btn;
	fm.collocate();

	//Show the form
	fm.show();

	//Start to event loop process, it blocks until the form is closed.
	exec();
}