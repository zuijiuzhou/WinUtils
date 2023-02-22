#include <iostream>
#include <boost/program_options.hpp>
#include <algorithm>
#include <argpase/argparse.hpp>
#include "evarutil.h"
#include "clslogger.h"
#include "pathutil.h"
#include "shutil.h"
#include "mp_global.h"

using Parser = argparse::ArgumentParser;
using Argument = argparse::Argument;
namespace bpo = boost::program_options;

#define ENVVAR_PATH "PATH"
#define DEFAULT_ENVVAR_NAME ENVVAR_PATH

#define __CMD_GET__ "get"
#define __CMD_SET__ "set"
#define __CMD_DEL__ "del"

#define __ARG_HELP__ "--help"
#define __ARG_HELP2__ "-h"
#define __ARG_GLOBAL__ "--global"
#define __ARG_GLOBAL2__ "-g"
#define __ARG_NAME__ "--name"
#define __ARG_NAME2__ "-n"

#define __ARG_SPLIT__ "--split"
#define __ARG_SPLIT2__ "-x"
#define __ARG_SLIM__ "--slim"
#define __ARG_SLIM2__ "-r"
#define __ARG_SORT_ASC__ "--sort-asc"
#define __ARG_SORT_ASC2__ "-sa"
#define __ARG_SORT_DESC__ "--sort-desc"
#define __ARG_SORT_DESC2__ "-sd"

#define __ARG_VALUE__ "--value"
#define __ARG_VALUE2__ "-v"
#define __ARG_VALUE_PLUS__ "--value+"
#define __ARG_VALUE_PLUS2__ "-v+"
#define __ARG_VALUE_MINUS__ "--value-"
#define __ARG_VALUE_MINUS2__ "-v-"
#define __ARG_SAVE__ "--save"
#define __ARG_SAVE2__ "-s"

static Parser cmd_root(__APPLICATION_NAME__);
static Parser cmd_get(__CMD_GET__);
static Parser cmd_set(__CMD_SET__);
static Parser cmd_del(__CMD_DEL__);

void def_common_args(Parser &cmd);
void def_format_args(Parser &cmd);
void get_common_args(Parser &parser, bool *global, std::string *name);
void get_format_args(Parser &parser, bool *split, bool *slim, bool *sort_asc, bool *sort_desc);
std::wstring get_env_val(const std::string &name, bool global);
void process_env_val(std::vector<std::wstring> &vals, bool slim, bool sort_asc, bool sort_desc);
std::vector<std::wstring> process_env_val(const std::wstring &val, bool slim, bool sort_asc, bool sort_desc);
int cmd_get_handler(Parser &cmd);
int cmd_set_handler(Parser &cmd);
Parser &build_arg_parser();

void def_common_args(Parser &cmd)
{
    cmd.add_argument(__ARG_GLOBAL2__, __ARG_GLOBAL__).default_value(false).implicit_value(true).nargs(0).help("此开关控制相关操作作用到系统环境变量");
    cmd.add_argument(__ARG_NAME2__, __ARG_NAME__).default_value(DEFAULT_ENVVAR_NAME).nargs(1).help("环境变量名称");
}

void def_format_args(Parser &cmd)
{
    cmd.add_argument(__ARG_SPLIT2__, __ARG_SPLIT__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否换行显示环境变量的每个值");
    cmd.add_argument(__ARG_SLIM2__, __ARG_SLIM__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否去重");
    cmd.add_argument(__ARG_SORT_ASC2__, __ARG_SORT_ASC__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否升序排序环境变量");
    cmd.add_argument(__ARG_SORT_DESC2__, __ARG_SORT_DESC__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否降序排序环境变量");
}

void get_common_args(Parser &parser, bool *global, std::string *name)
{
    if (parser.is_used(__ARG_GLOBAL__))
    {
        *global = parser.get<bool>(__ARG_GLOBAL__);
    }
    if (parser.is_used(__ARG_NAME__))
    {
        *name = parser.get<std::string>(__ARG_NAME__);
        if(name->empty()){
            throw std::exception("no evar name specified");
        }
    }
    else{
        *name = DEFAULT_ENVVAR_NAME;
    }
}

void get_format_args(Parser &cmd, bool *split, bool *slim, bool *sort_asc, bool *sort_desc)
{
    if (cmd.is_used(__ARG_SPLIT__))
    {
        *split = cmd.get<bool>(__ARG_SPLIT__);
    }
    if (cmd.is_used(__ARG_SLIM__))
    {
        *slim = cmd.get<bool>(__ARG_SLIM__);
    }
    if (cmd.is_used(__ARG_SORT_ASC__))
    {
        *sort_asc = cmd.get<bool>(__ARG_SORT_ASC__);
    }
    if (cmd.is_used(__ARG_SORT_DESC__))
    {
        *sort_desc = cmd.get<bool>(__ARG_SORT_DESC__);
    }
}

std::wstring get_env_val(std::string &name, bool global)
{
    std::wstring val = evar<std::wstring>(str_to_wstr(name), global);
    return val;
}

void process_env_val(std::vector<std::wstring> &vals, bool slim, bool sort_asc, bool sort_desc)
{
    auto &items = vals;
    if (slim)
    {
        std::vector<std::wstring> tempPaths;
        for (auto &p : items)
        {
            if (std::find_if(tempPaths.begin(), tempPaths.end(), [&p](std::wstring &i)
                             { return path_compare(p, i) == 0; }) == tempPaths.end())
            {
                tempPaths.push_back(p);
            }
        }
        items.assign(tempPaths.begin(), tempPaths.end());
    }
    if (sort_asc)
    {
        std::sort(items.begin(), items.end(), [](const std::wstring &left, const std::wstring &right)
                  { return tstr_cmp_icase(left, right) < 0; });
    }
    else if (sort_desc)
    {
        std::sort(items.begin(), items.end(), [](const std::wstring &left, const std::wstring &right)
                  { return tstr_cmp_icase(left, right) > 0; });
    }
}

std::vector<std::wstring> process_env_val(const std::wstring &val, bool slim, bool sort_asc, bool sort_desc)
{
    std::vector<std::wstring> items = path_split(val);
    process_env_val(items, slim, sort_asc, sort_desc);
    return items;
}

int cmd_get_handler(Parser &cmd)
{
    bool global = false, split = false, slim = false, sort_asc = false, sort_desc = false;
    std::string name;
    get_common_args(cmd, &global, &name);
    get_format_args(cmd, &split, &slim, &sort_asc, &sort_desc);
    if(false == evar_exists(name, global)){
        throw std::exception("the evar specified does not exist");
    }
    std::wstring val = get_env_val(name, global);
    if (split || slim || sort_asc || sort_desc)
    {
        std::vector<std::wstring> vals = process_env_val(val, slim, sort_asc, sort_desc);
        if (split)
        {
            for (auto &item : vals)
            {
                clslog(item, MT_TEXT);
            }
        }
        else
        {
            clslog(path_join(vals), MT_TEXT);
        }
    }
    else
    {
        clslog(val, MT_TEXT);
    }
    return 0;
}

int cmd_set_handler(Parser &cmd)
{
    bool global = false, split = false, slim = false, sort_asc = false, sort_desc = false, save = false;
    std::string name, value, value_plus, value_minus;
    get_common_args(cmd, &global, &name);
    get_format_args(cmd, &split, &slim, &sort_asc, &sort_desc);

    if (cmd.is_used(__ARG_SAVE__))
    {
        save = cmd.get<bool>(__ARG_SAVE__);
    }
    if (cmd.is_used(__ARG_VALUE__))
    {
        value = cmd.get<std::string>(__ARG_VALUE__);
    }
    if (cmd.is_used(__ARG_VALUE_PLUS__))
    {
        value_plus = cmd.get<std::string>(__ARG_VALUE_PLUS__);
    }
    if (cmd.is_used(__ARG_VALUE_MINUS__))
    {
        value_minus = cmd.get<std::string>(__ARG_VALUE_MINUS__);
    }

    std::wstring val;
    std::wstring val_processed;

    if (false == value.empty())
    {
        val = str_to_wstr(value);
    }
    else
    {
        val = get_env_val(name, global);
    }
    std::vector<std::wstring> vals = process_env_val(val, slim, sort_asc, sort_desc);
    if (false == value_plus.empty())
    {
        vals.push_back(str_to_wstr(value_plus));
        process_env_val(vals, slim, sort_asc, sort_desc);
    }
    else
    {
        value_minus = path_norm(value_minus);
        std::wstring value_minus_normalized = str_to_wstr(path_norm(value_minus));
        auto foundAt = std::find_if(vals.begin(), vals.end(), [&value_minus_normalized](const std::wstring &item)
                                    { return path_compare(value_minus_normalized, item) == 0; });
        if (foundAt != vals.end())
        {
            vals.erase(foundAt);
        }
        
    }
    val_processed = path_join(vals);
    if (split)
    {
        for (auto &item : vals)
        {
            clslog(item, MT_TEXT);
        }
    }
    else
    {
        clslog(val_processed, MT_TEXT);
    }
    if (save)
    {
        try
        {
            if (global)
            {
                if (!IsUserAnAdmin())
                {
                    clslog("\ncan not save the value, please start as administrator", MT_ERR);
                    return -1;
                }
            }
            evar_set(str_to_wstr(name), val_processed, name == ENVVAR_PATH ? REG_EXPAND_SZ : REG_SZ, global);
            clslog("SUCCESS", MT_INFO);
            return 0;
        }
        catch (const std::exception &ex)
        {
            clslog(ex.what(), MT_ERR);
            return 9;
        }
        catch (...)
        {
            clslog("未知错误", MT_ERR);
            return 2;
        }
    }
    return 0;
}

Parser &build_arg_parser()
{

    // app.add_argument(__ARG_HELP2__, __ARG_HELP__).help("显示命令行帮助");

    def_common_args(cmd_get);
    def_format_args(cmd_get);

    def_common_args(cmd_set);
    def_format_args(cmd_set);
    cmd_set.add_argument(__ARG_VALUE2__, __ARG_VALUE__).help("设置或修改环境变量值");
    cmd_set.add_argument(__ARG_VALUE_PLUS2__, __ARG_VALUE_PLUS__).help("追加环境变量值,指定了value选项时无效");
    cmd_set.add_argument(__ARG_VALUE_MINUS2__, __ARG_VALUE_MINUS__).help("移除环境变量值,指定了value选项时无效");
    cmd_set.add_argument(__ARG_SAVE2__, __ARG_SAVE__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否保存");

    def_common_args(cmd_del);

    cmd_root.add_subparser(cmd_get);
    cmd_root.add_subparser(cmd_set);
    cmd_root.add_subparser(cmd_del);

    return cmd_root;
}

int main(int argc, char **argv)
{
    char *locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");

    auto &parser = build_arg_parser();
    if (argc == 1)
    {
        clslog(parser.usage(), MT_TEXT);
        return 1;
    }
    try
    {
        parser.parse_args(argc, argv);
        // if (parser.is_used(__ARG_HELP__))
        // {
        //     clslog(parser.usage(), MT_TEXT);
        //     return 0;
        // }
        if (parser.is_subcommand_used(__CMD_GET__))
        {
            cmd_get_handler(cmd_get);
        }
        else if (parser.is_subcommand_used(__CMD_SET__))
        {
            cmd_set_handler(cmd_set);
        }
    }
    catch (const std::exception &e)
    {
        clslog(e.what(), MT_ERR);
    }

    return 0;
}

int main000(int argc, char **argv)
{
    char *locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    std::string
        name,       // 环境变量名
        val,        // 设置的环境变量值
        val_append, // 追加的环境变量值
        val_remove; // 移除的环境变量值
    bool
        global = 0,    // 1:系统环境
        sort_asc = 0,  // 1:升序
        sort_desc = 0, // 1:降序
        split = 0,     // 1:分割换行显示
        slim = 0,      // 1:去重
        save = 0,      // 1:保存
        del = 0;       // 1:移除

    bpo::variables_map vm;
    bpo::options_description opts_desc("Usage");
    opts_desc.add_options()("help,h", "显示命令行帮助")("name,n", bpo::value(&name)->default_value(DEFAULT_ENVVAR_NAME), "环境变量名称")("val", bpo::value(&val), "环境变量值")("val+", bpo::value(&val_append), "追加环境变量值,指定了value选项时无效")("val-", bpo::value(&val_remove), "移除环境变量值,指定了value选项时无效")("del", bpo::bool_switch(&del)->default_value(false), "移除指定名称的环境变量")("global,g", bpo::bool_switch(&global)->default_value(false), "此开关控制相关操作作用到系统环境变量")("sort-asc,a", bpo::bool_switch(&sort_asc)->default_value(false), "此开关控制是否升序排序环境变量")("sort-desc,d", bpo::bool_switch(&sort_desc)->default_value(false), "此开关控制是否降序排序环境变量")("split,x", bpo::bool_switch(&split)->default_value(false), "此开关控制是否换行显示环境变量的每个值")("slim,r", bpo::bool_switch(&slim)->default_value(false), "此开关控制是否去重")("save,s", bpo::bool_switch(&save)->default_value(false), "此开关控制是否保存");

    try
    {
        bpo::store(bpo::command_line_parser(argc, argv).options(opts_desc).run(), vm);
        bpo::notify(vm);
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what();
        return -1;
    }
    if (vm.count("name"))
    {
        if (name.empty())
        {
            return -2;
        }
    }
    else
    {
        if (name.empty())
        {
            name.assign(ENVVAR_PATH);
        }
    }
    if (vm.count("help"))
    {
        std::cout << opts_desc << std::endl;
        return 0;
    }

    std::wstring path, path_result;
    std::vector<std::wstring> paths;
    if (vm.count("value"))
    {
        path = str_to_wstr(val);
        if (path.length() == 0)
        {
            clslog("the env val inputted is empty", MT_WARN);
        }
    }
    else
    {
        try
        {
            path = evar(str_to_wstr(name), global);
        }
        catch (const std::exception &ex)
        {
            clslog(ex.what(), MT_ERR);
            return -999;
        }
    }

    if (split || slim || sort_asc || sort_desc)
    {
        paths = path_split(path);
        if (slim)
        {
            std::vector<std::wstring> tempPaths;
            for (auto &p : paths)
            {
                if (std::find_if(tempPaths.begin(), tempPaths.end(), [&p](std::wstring &i)
                                 { return path_compare(p, i) == 0; }) == tempPaths.end())
                {
                    tempPaths.push_back(p);
                }
            }
            paths.assign(tempPaths.begin(), tempPaths.end());
        }
        if (sort_asc)
        {
            std::sort(paths.begin(), paths.end(), [](const std::wstring &left, const std::wstring &right)
                      { return tstr_cmp_icase(left, right) < 0; });
        }
        else if (sort_desc)
        {
            std::sort(paths.begin(), paths.end(), [](const std::wstring &left, const std::wstring &right)
                      { return tstr_cmp_icase(left, right) > 0; });
        }
        path_result = path_join(paths);
    }
    else
    {
        path_result = path;
    }
    if (split)
    {
        for (auto &p : paths)
        {
            std::wcout << p << std::endl;
        }
    }
    else
    {
        std::wcout << path_result;
    }
    if (save)
    {
        try
        {
            if (global)
            {
                if (!IsUserAnAdmin())
                {
                    clslog("\ncan not save the value, please start as administrator", MT_ERR);
                    return -1;
                }
            }
            evar_set(str_to_wstr(name), path_result, name == ENVVAR_PATH ? REG_EXPAND_SZ : REG_SZ, global);
            clslog("SUCCESS", MT_INFO);
            return 0;
        }
        catch (const std::exception &ex)
        {
            clslog(ex.what(), MT_ERR);
            return 9;
        }
    }
    return 0;
}