#include "config.h"
#include "global.h"
#include "ArgvQuote.h"


void run()
{
	if (run_by == 1) {
		MINILOG0("run using execv()");

		char** script_argv = original_argv + script_pos;

		//const char* env[] = { R"(PATH=F:\vcpkg\installed\x86-windows\bin)", nullptr };
		const char* const env[] = { exec_cmd_env_vars.c_str(), nullptr };

#ifdef _WIN32
		for (int i = 0; i < script_argc; i++) {
			//cout << "script_argv[" << i << "]: " << script_argv[i] << endl;
			string argument{ script_argv[i] };
			string quoted_argument;
			ArgvQuote(argument, quoted_argument, false);
			script_argv[i] = new char[quoted_argument.size() + 1];
			strcpy(script_argv[i], quoted_argument.c_str());
		}
		assert(script_argv[script_argc] == nullptr);


		// Windows没有exec功能，但有个_exec函数来模拟exec。
		// 可是_exec创建了一个子进程，往往子进程还没有结束，父进程就已经结束了。
		// 导致命令行窗口的输出混乱。所以我们用_spawnv。
		_spawnve(_P_WAIT, exe_path.string().c_str(), script_argv, env);
#else
		execve(exe_path.c_str(), script_argv, const_cast<char* const *>(env));
#endif
	}
	else if (run_by == 0) {
		MINILOG0("run using system()");
		string script_args;

		//        if (vm.count("args")) {
		//            for (auto a : vm["args"].as<vector<string>>()) {
		//                script_args += " '"; // 把脚本的参数用单引号括起来，避免通配符展开。该展开的通配符在解释器执行时已经展开过了
		//                script_args += a;
		//                script_args += "'";
		//            }
		//        }

		for (int i = 1; i < script_argc; i++) {
			script_args += " '"; // 把脚本的参数用单引号括起来，避免通配符展开。该展开的通配符在解释器执行时已经展开过了
			script_args += original_argv[script_pos + i];
			script_args += "'";
		}

		string cmd_line = exe_path.string();
		cmd_line += script_args;
		MINILOG0("final cmd line: " << cmd_line);
		// 运行产生的可执行文件
		system(cmd_line.c_str());
	}

}
