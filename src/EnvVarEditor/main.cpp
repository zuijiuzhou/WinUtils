#include <iostream>
#include <boost/program_options.hpp>
#include <algorithm>
#include "evarutil.h"
#include "clslogger.h"
#include "pathutil.h"
#include "shutil.h"

namespace bpo = boost::program_options;
#define ENVVAR_PATH "PATH"
#define DEFAULT_ENVVAR_NAME ENVVAR_PATH

int main(int argc, char **argv)
{

    char *locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    std::string name, val;
    bool global = 0, sort_asc = 0, sort_desc = 0, split = 0, slim = 0, save = 0;

    bpo::variables_map vm;
    bpo::options_description opts_desc("Usage");

    opts_desc.add_options()("help,h", "显示命令行帮助")("name,n", bpo::value(&name)->default_value(DEFAULT_ENVVAR_NAME), "环境变量名称")("value,v", bpo::value(&val), "环境变量值")("global,g", bpo::bool_switch(&global)->default_value(false), "此开关控制相关操作作用到系统环境变量")("sort-asc,a", bpo::bool_switch(&sort_asc)->default_value(false), "此开关控制是否升序排序环境变量")("sort-desc,d", bpo::bool_switch(&sort_desc)->default_value(false), "此开关控制是否降序排序环境变量")("split", bpo::bool_switch(&split)->default_value(false), "此开关控制是否换行显示环境变量的每个值")("slim,x", bpo::bool_switch(&slim)->default_value(false), "此开关控制是否去重")("save,s", bpo::bool_switch(&save)->default_value(false), "此开关控制是否保存");

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
        path = string2Wstring(val);
        if (path.length() == 0)
        {
            clslog("the env val inputted is empty", MT_WARN);
        }
    }
    else
    {
        try
        {
            path = evGet(string2Wstring(name), global);
        }
        catch (const std::exception &ex)
        {
            clslog(ex.what(), MT_ERR);
            return -999;
        }
    }

    if (split || slim || sort_asc || sort_desc)
    {
        paths = pathSplit(path);
        if (slim)
        {
            std::vector<std::wstring> tempPaths;
            for (auto &p : paths)
            {
                if (std::find_if(tempPaths.begin(), tempPaths.end(), [&p](std::wstring &i)
                                 { return pathCompare(p, i) == 0; }) == tempPaths.end())
                {
                    tempPaths.push_back(p);
                }
            }
            paths.assign(tempPaths.begin(), tempPaths.end());
        }
        if (sort_asc)
        {
            std::sort(paths.begin(), paths.end(), [](const std::wstring &left, const std::wstring &right)
                      { return tstrCompareIgnoreCase(left, right) < 0; });
        }
        else if (sort_desc)
        {
            std::sort(paths.begin(), paths.end(), [](const std::wstring &left, const std::wstring &right)
                      { return tstrCompareIgnoreCase(left, right) > 0; });
        }
        path_result = pathJoin(paths);
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
            evSet(string2Wstring(name), path_result, name == ENVVAR_PATH ? REG_EXPAND_SZ : REG_SZ, global);
            clslog("Success", MT_INFO);
            return 0;
        }
        catch (const std::exception &ex)
        {
            clslog(ex.what(), MT_ERR);
            return 9;
        }
    }
}