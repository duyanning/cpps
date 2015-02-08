#include "std.h"
#include "Cpp2ObjAction.h"

void Cpp2ObjAction::execute(PDependencyGraphEntity target,
                            vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre)
{
}
    // def execute(self, target, allPre, changedPre):
    //     Action.execute(self, target, allPre, changedPre)

    //     cpp_path = allPre[0].path()
    //     obj_path = target.path()
    //     dep_path = obj_path.replace(".o", ".d")

    //     if self.m_prj.type() == "dll":
    //         cmd = "g++ -Wall -c -fPIC -o"
    //     else:
    //         cmd = "g++ -Wall -c -o"

    //     cmd += " " + obj_path

    //     for p in allPre:
    //         if p.path().endswith(".cpp"):
    //             cmd += " " + p.path()

    //     optMgr = OptionManager()
    //     options = optMgr.getOptionFor(cpp_path, self.m_prj.activeConfig())

    //     if not options == "":
    //         cmd = cmd + " " + options

    //     cmd += " -fpch-deps -MMD -MF " + dep_path

    //     #print cmd
    //     print "Compiling " + os.path.basename(cpp_path) + " ..."
    //     #os.system(cmd)
    //     retcode = call(cmd, shell=True)
    //     if retcode != 0:
    //         self.m_prj.setCompileOk(False)
