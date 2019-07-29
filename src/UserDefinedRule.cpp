#include "config.h"
#include "global.h"
#include "UserDefinedRule.h"
#include "InfoPackageScanned.h"

// 各部分的命名可参考 https://www.gnu.org/software/make/manual/make.html#Rule-Introduction
void process_user_defined_rule(fs::path src_path, string rule_string, InfoPackageScanned& pack)
{
    //cout << rule_string << endl;
    // 形如 ui.cpp ui.h : ui.fl // fluid -c ui.fl
    // ui.cpp ui.h : ui.fl 叫dependency relationship。冒号前的叫targets，冒号后的叫prerequisites
    // fluid -c ui.fl 叫recipe，可由多个command组成

    // 分离dependency relationship与recipe部分
    auto slash_slash_pos = rule_string.find("//");
    string dependency_relationship = rule_string.substr(0, slash_slash_pos);
    boost::algorithm::trim(dependency_relationship);
    string recipe = rule_string.substr(slash_slash_pos + 2);
    boost::algorithm::trim(recipe);
    //cout << dependency_relationship << endl;
    //cout << recipe << endl;

    // 分离dependency relationship中的targets与prerequisites
    auto colon_pos = dependency_relationship.find(":");
    string targets = dependency_relationship.substr(0, colon_pos);
    boost::algorithm::trim(targets);
    string prerequisites = dependency_relationship.substr(colon_pos + 1);
    boost::algorithm::trim(prerequisites);
    //cout << targets << endl;
    //cout << prerequisites << endl;

    // 分离dependency relationship中的各个target
    vector<string> target_vector;
    std::string target_delimiters(" "); // 支持多个分界符
    boost::split(target_vector, targets, boost::is_any_of(target_delimiters), boost::token_compress_on); // token_compress_on是为了将单词之间的多个空格视为一个
    //for (auto t : target_vector) {
    //    cout << ">>>" << t << "<<<" << endl;
    //}

    // 分离prerequisites中的各个prerequisite
    vector<string> prerequisite_vector;
    std::string prerequisite_delimiters(" "); // 支持多个分界符
    boost::split(prerequisite_vector, prerequisites, boost::is_any_of(prerequisite_delimiters), boost::token_compress_on); // token_compress_on是为了将单词之间的多个空格视为一个
    //for (auto p : prerequisite_vector) {
    //    cout << ">>>" << p << "<<<" << endl;
    //}

    UserDefinedRule rule;
    rule.dir = src_path;
    rule.dir.remove_filename();

    for (auto t : target_vector) {
        fs::path a = rule.dir;
        a /= t;
        rule.targets.push_back(a);
    }

    for (auto p : prerequisite_vector) {
        fs::path a = rule.dir;
        a /= p;
        rule.prerequisites.push_back(a);
    }

    {
        rule.commands.push_back(recipe);
    }

    pack.user_defined_rules.push_back(rule);

}

void process_user_defined_rule_multi_lines(fs::path src_path, string dependency_relationship,
    vector<string> commands, InfoPackageScanned& pack)
{
    // 分离dependency relationship中的targets与prerequisites
    auto colon_pos = dependency_relationship.find(":");
    string targets = dependency_relationship.substr(0, colon_pos);
    boost::algorithm::trim(targets);
    string prerequisites = dependency_relationship.substr(colon_pos + 1);
    boost::algorithm::trim(prerequisites);
    //cout << targets << endl;
    //cout << prerequisites << endl;

    // 分离dependency relationship中的各个target
    vector<string> target_vector;
    std::string target_delimiters(" "); // 支持多个分界符
    boost::split(target_vector, targets, boost::is_any_of(target_delimiters), boost::token_compress_on); // token_compress_on是为了将单词之间的多个空格视为一个
    //for (auto t : target_vector) {
    //    cout << ">>>" << t << "<<<" << endl;
    //}

    // 分离prerequisites中的各个prerequisite
    vector<string> prerequisite_vector;
    std::string prerequisite_delimiters(" "); // 支持多个分界符
    boost::split(prerequisite_vector, prerequisites, boost::is_any_of(prerequisite_delimiters), boost::token_compress_on); // token_compress_on是为了将单词之间的多个空格视为一个
    //for (auto p : prerequisite_vector) {
    //    cout << ">>>" << p << "<<<" << endl;
    //}

    UserDefinedRule rule;
    rule.dir = src_path;
    rule.dir.remove_filename();

    for (auto t : target_vector) {
        fs::path a = rule.dir;
        a /= t;
        rule.targets.push_back(a);
    }

    for (auto p : prerequisite_vector) {
        fs::path a = rule.dir;
        a /= p;
        rule.prerequisites.push_back(a);
    }

    
    rule.commands = commands;

    pack.user_defined_rules.push_back(rule);

}