#include <iostream>
#include <algorithm>
#include <argpase/argparse.hpp>
#include "evarutil.h"
#include "clslogger.h"
#include "pathutil.h"
#include "shutil.h"
#include "mp_global.h"

#ifdef __BOOST_ENABLED__
#include <boost/program_options.hpp>
namespace bpo = boost::program_options;
#endif

using Parser = argparse::ArgumentParser;
using Argument = argparse::Argument;

#define ENVVAR_PATH "PATH"
#define DEFAULT_ENVVAR_NAME ENVVAR_PATH

#define __CMD_GET__ "get"
#define __CMD_SET__ "set"
#define __CMD_DEL__ "del"
#define __CMD_EXPORT__ "export"
#define __CMD_IMPORT__ "import"

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

#define __ARG_OUPUT__ "--output"
#define __ARG_OUPUT2__ "-o"

#define __ARG_INPUT__ "--input"
#define __ARG_INPUT2__ "-i"
#define __ARG_OVERRIDE__ "--override"

#define __SUCCESS__ 0

#define __ERR_UNKONW__ 0x1
#define __ERR_NAME_NOT_SPECIFIED__ 0x1000
#define __ERR_NAME_NOT_EXISTED__ 0x1001
#define __ERR_REG_READ_FAILD__ 0x1101
#define __ERR_REG_SET_FAILD__ 0x1102
#define __ERR_REG_DEL_FAILD__ 0x1103
#define __ERR_ADMIN_REQUIRED__ 0x1201
#define __ERR_PARAM_MISMATCH__ 0x1202

static Parser cmd_root(__APPLICATION_NAME__);
static Parser cmd_get(__CMD_GET__);
static Parser cmd_set(__CMD_SET__);
static Parser cmd_del(__CMD_DEL__);
static Parser cmd_export(__CMD_EXPORT__);
static Parser cmd_import(__CMD_IMPORT__);
static std::string err_msg_inner;

void def_common_args(Parser &cmd, bool def_name = true);
void def_format_args(Parser &cmd);
int get_common_args(Parser &parser, bool *global, std::string *name);
int get_format_args(Parser &parser, bool *split, bool *slim, bool *sort_asc, bool *sort_desc);
int get_env_val(const std::string &name, bool global, std::wstring &o_val);
void process_env_val(std::vector<std::wstring> &vals, bool slim, bool sort_asc, bool sort_desc);
std::vector<std::wstring> process_env_val(const std::wstring &val, bool slim, bool sort_asc, bool sort_desc);
int cmd_get_handler(Parser &cmd);
int cmd_set_handler(Parser &cmd);
int cmd_del_handler(Parser &cmd);
int cmd_export_handler(Parser &cmd);
int cmd_import_handler(Parser &cmd);
Parser &build_arg_parser();
const char *get_err_desc(int code);

void def_common_args(Parser &cmd, bool def_name)
{
    auto &arg_g = cmd.add_argument(__ARG_GLOBAL2__, __ARG_GLOBAL__).default_value(false).implicit_value(true).nargs(0).help("此开关控制相关操作作用到系统环境变量");
    auto &arg_n = cmd.add_argument(__ARG_NAME2__, __ARG_NAME__).required().nargs(1).help("环境变量名称");
    if (def_name)
    {
        arg_n.default_value(DEFAULT_ENVVAR_NAME);
    }
}

void def_format_args(Parser &cmd)
{
    cmd.add_argument(__ARG_SPLIT2__, __ARG_SPLIT__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否换行显示环境变量的每个值");
    cmd.add_argument(__ARG_SLIM2__, __ARG_SLIM__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否去重");
    cmd.add_argument(__ARG_SORT_ASC2__, __ARG_SORT_ASC__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否升序排序环境变量");
    cmd.add_argument(__ARG_SORT_DESC2__, __ARG_SORT_DESC__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否降序排序环境变量");
}

int get_common_args(Parser &parser, bool *global, std::string *name)
{
    if (parser.is_used(__ARG_GLOBAL__))
    {
        *global = parser.get<bool>(__ARG_GLOBAL__);
    }
    if (parser.is_used(__ARG_NAME__))
    {
        *name = parser.get<std::string>(__ARG_NAME__);
        if (name->empty())
        {
            return __ERR_NAME_NOT_SPECIFIED__;
        }
    }
    else
    {
        *name = parser.get<std::string>(__ARG_NAME__);
    }
    return 0;
}

int get_format_args(Parser &cmd, bool *split, bool *slim, bool *sort_asc, bool *sort_desc)
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
    return 0;
}

int get_env_val(std::string &name, bool global, std::wstring &o_val)
{
    try
    {
        o_val = evar<std::wstring>(str_to_wstr(name), global);
    }
    catch (const std::exception &e)
    {
        err_msg_inner = e.what();
        return __ERR_REG_READ_FAILD__;
    }
    catch (...)
    {
        return __ERR_REG_READ_FAILD__;
    }
    return 0;
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
    int iret = 0;
    bool global = false, split = false, slim = false, sort_asc = false, sort_desc = false;
    std::string name;
    iret = get_common_args(cmd, &global, &name);
    if (iret)
        return iret;
    iret = get_format_args(cmd, &split, &slim, &sort_asc, &sort_desc);
    if (iret)
        return iret;

    if (false == evar_exists(name, global))
    {
        return __ERR_NAME_NOT_EXISTED__;
    }
    std::wstring val;
    iret = get_env_val(name, global, val);
    if (iret)
        return iret;
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
    int iret;
    bool global = false, split = false, slim = false, sort_asc = false, sort_desc = false, save = false;
    std::string name, value, value_plus, value_minus;
    iret = get_common_args(cmd, &global, &name);
    if (iret)
        return iret;
    iret = get_format_args(cmd, &split, &slim, &sort_asc, &sort_desc);
    if (iret)
        return iret;

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
        iret = get_env_val(name, global, val);
        if (iret)
            return iret;
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
                if (false == IsUserAnAdmin())
                {
                    return __ERR_ADMIN_REQUIRED__;
                }
            }
            evar_set(str_to_wstr(name), val_processed, name == ENVVAR_PATH ? REG_EXPAND_SZ : REG_SZ, global);
            clslog("SUCCESS", MT_INFO);
            return __SUCCESS__;
        }
        catch (const std::exception &e)
        {
            err_msg_inner = e.what();
            return __ERR_REG_SET_FAILD__;
        }
        catch (...)
        {
            return __ERR_REG_SET_FAILD__;
        }
    }
    return __SUCCESS__;
}

int cmd_del_handler(Parser &cmd)
{
    int iret = __SUCCESS__;
    bool global = false;
    std::string name;
    iret = get_common_args(cmd, &global, &name);
    if (iret)
        return iret;

    if (false == evar_exists(name, global))
    {
        return __ERR_NAME_NOT_EXISTED__;
    }

    try
    {
        evar_del(name, global);
        clslog("SUCCESS", MT_INFO);
    }
    catch (const std::exception &e)
    {
        err_msg_inner = e.what();
        return __ERR_REG_DEL_FAILD__;
    }
    catch (...)
    {
        return __ERR_REG_DEL_FAILD__;
    }
    return __SUCCESS__;
}

int cmd_export_handler(Parser &cmd)
{
    std::string output;

    if (cmd.is_used(__ARG_OUPUT__))
    {
        output = cmd.get<std::string>(__ARG_OUPUT__);
    }
    else
    {
    }
    // FILE *f = fopen(, );
    return 0;
}

int cmd_import_handler(Parser &cmd)
{

    return 0;
}

Parser &build_arg_parser()
{

    // app.add_argument(__ARG_HELP2__, __ARG_HELP__).help("显示命令行帮助");

    // GET
    def_common_args(cmd_get);
    def_format_args(cmd_get);

    // SET
    def_common_args(cmd_set);
    def_format_args(cmd_set);
    cmd_set.add_argument(__ARG_VALUE2__, __ARG_VALUE__).help("设置或修改环境变量值");
    cmd_set.add_argument(__ARG_VALUE_PLUS2__, __ARG_VALUE_PLUS__).help("追加环境变量值,指定了value选项时无效");
    cmd_set.add_argument(__ARG_VALUE_MINUS2__, __ARG_VALUE_MINUS__).help("移除环境变量值,指定了value选项时无效");
    cmd_set.add_argument(__ARG_SAVE2__, __ARG_SAVE__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否保存");

    // DEL
    def_common_args(cmd_del, false);
    // EXPORT
    cmd_export.add_argument(__ARG_OUPUT2__, __ARG_OUPUT__).required().nargs(1).help("环境变量保存路径");

    // IMPORT
    cmd_import.add_argument(__ARG_INPUT2__, __ARG_INPUT__).required().nargs(1).help("环境变量文件路径");
    cmd_import.add_argument(__ARG_OVERRIDE__).default_value(false).implicit_value(true).nargs(0).help("此开关控制是否覆盖已存在的环境变量值");

    cmd_root.add_subparser(cmd_get);
    cmd_root.add_subparser(cmd_set);
    cmd_root.add_subparser(cmd_del);
    cmd_root.add_subparser(cmd_export);
    cmd_root.add_subparser(cmd_import);

    return cmd_root;
}

const char *get_err_desc(int code)
{
    if (code == __ERR_NAME_NOT_SPECIFIED__)
    {
        return "No environment variable name specified";
    }
    else if (code == __ERR_NAME_NOT_EXISTED__)
    {
        return "The specified name does not exist";
    }
    else if (code == __ERR_REG_READ_FAILD__)
    {
        return "Failed to read registry";
    }
    else if (code == __ERR_REG_SET_FAILD__)
    {
        return "Failed to write registry";
    }
    else if (code == __ERR_REG_DEL_FAILD__)
    {
        return "Failed to delete registry";
    }
    else if (code == __ERR_ADMIN_REQUIRED__)
    {
        return "Please start with administrator privileges";
    }
    else if (code == __ERR_PARAM_MISMATCH__)
    {
        return "Command line argument error";
    }
    return "Unknown error";
}

int main(int argc, char **argv)
{
    char *locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");

    auto &parser = build_arg_parser();
    int iret = __SUCCESS__;
    if (argc == 1)
    {
        clslog(cmd_root.usage(), MT_TEXT);
        return -1;
    }
    try
    {
        parser.parse_args(argc, argv);
        try
        {
            if (parser.is_subcommand_used(__CMD_GET__))
            {
                iret = cmd_get_handler(cmd_get);
            }
            else if (parser.is_subcommand_used(__CMD_SET__))
            {
                iret = cmd_set_handler(cmd_set);
            }
            else if (parser.is_subcommand_used(__CMD_DEL__))
            {
                iret = cmd_del_handler(cmd_del);
            }
            else if (parser.is_subcommand_used(__CMD_EXPORT__))
            {
                iret = cmd_export_handler(cmd_export);
            }
            else if (parser.is_subcommand_used(__CMD_IMPORT__))
            {
                iret = cmd_import_handler(cmd_import);
            }
        }
        catch (const std::exception &e)
        {
            err_msg_inner = e.what();
            iret = __ERR_UNKONW__;
        }
        catch (...)
        {
            iret = __ERR_UNKONW__;
        }
    }
    catch (const std::exception &e)
    {
        err_msg_inner = e.what();
        iret = __ERR_PARAM_MISMATCH__;
    }
    catch (...)
    {
        iret = __ERR_PARAM_MISMATCH__;
    }

    if (iret)
    {
        clslog(get_err_desc(iret), MT_ERR);
        // #ifdef DEBUG
        if (err_msg_inner.length())
        {
            clslog(err_msg_inner, MT_ERR);
        }
        // #endif
    }
    return iret;
}

#ifdef __BOOST_ENABLED__
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
#endif