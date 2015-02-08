#include "std.h"
#include "Obj2ExeAction.h"

void Obj2ExeAction::execute(PDependencyGraphEntity target,
                            vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre)
{
}
//     def execute(self, target, allPre, changedPre):
//         if not self.m_prj.getCompileOk():
//             return

//         Action.execute(self, target, allPre, changedPre)

//         exe_path = target.path()
//         if self.m_prj.type() == "dll":
//             cmd = "g++ -shared -fPIC -o "
//         elif self.m_prj.type() == "lib":
//             cmd = "ar rcs "
//         else:
//             cmd = "g++ -o "
//         cmd += exe_path
//         for p in allPre:
//             if p.path().endswith(".o"):
//                 cmd = cmd + " " + p.path()

// #         for lib in self.m_prj.librariesList():
// #             cmd = cmd + "-l" + lib + " "


//         optMgr = OptionManager()
//         options = optMgr.getOptionFor(exe_path, self.m_prj.activeConfig())

//         if not options == "":
//             cmd = cmd + " " + options

//         print "Linking..."
//         #print cmd
//         #os.system(cmd)
//         retcode = call(cmd, shell=True)
//         if retcode != 0:
//             self.m_prj.setLinkOk(False)
