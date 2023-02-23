
<h1 align="center">
EnvVarEditor
</h1>

### Description:
<pre>
  用户查询、获取、格式化、删除编辑变量
</pre>
### Usage: 
<pre> EnvVarEditor [--help] [--version] {del,export,get,import,set}</pre>

### Optional arguments:
<pre>
   -h, --help    shows help message and exits
   -v, --version prints version information and exits
</pre>

### Subcommands:
<pre>
  del
  export
  get
  import
  set
</pre>

## Command: <i>get</i>
### Usage: 
<pre>EnvVarEditor get [--help] [--version] [--global] [--name VAR] [--split] [--slim] [--sort-asc] [--sort-desc]</pre>

### Optional arguments:
<pre>
  -h, --help            shows help message and exits 
  -v, --version         prints version information and exits 
  -g, --global          此开关控制相关操作作用到系统环境变量 
  -n, --name            环境变量名称 [default: "PATH"]
  -x, --split           此开关控制是否换行显示环境变量的每个值 
  -r, --slim            此开关控制是否去重 
  -sa, --sort-asc       此开关控制是否升序排序环境变量 
  -sd, --sort-desc      此开关控制是否降序排序环境变量
</pre>
### Examples:

1、获取系统'PATH'变量值，并分割、升序排序、去重，--name的默认值为'PATH',可以省略
<pre>
  EnvVarEditor get --global --name PATH --split --sort-asc --slim
  或
  EnvVarEditor get -g -x -sa -r
</pre>



## Command: set
### Usage: 
<pre>
    EnvVarEditor set [--help] [--version] [--global] [--name VAR] [--split] [--slim] 
        [--sort-asc] [--sort-desc] [--value VAR] [--value+ VAR] [--value- VAR] [--save]
</pre>

### Optional arguments:
<pre>
  -h, --help            shows help message and exits
  -v, --version         prints version information and exits
  -g, --global          此开关控制相关操作作用到系统环境变量
  -n, --name            环境变量名称 [default: "PATH"]
  -x, --split           此开关控制是否换行显示环境变量的每个值
  -r, --slim            此开关控制是否去重
  -sa, --sort-asc       此开关控制是否升序排序环境变量
  -sd, --sort-desc      此开关控制是否降序排序环境变量
  -v, --value           设置或修改环境变量值
  -v+, --value+         追加环境变量值,指定了value选项时无效
  -v-, --value-         移除环境变量值,指定了value选项时无效
  -s, --save            此开关控制是否保存
</pre>
### Examples:
1、新建名称为'JAVA_HOME'的系统变量
<pre>
  EnvVarEditor set -g --save -n JAVA_HOME -v c:\java 
</pre>

2、升序排序系统环境变量'PATH'的值并去重但不保存
<pre>
  EnvVarEditor set -g -r -sa
</pre>

3、在当前用户环境变量'PATH'中追加一个路径
<pre>
  EnvVarEditor set -s -v+ C:\Python
</pre>

## Command: del
### Usage: 
<pre>EnvVarEditor del [--help] [--version] [--global] [--name VAR]</pre>

### Optional arguments:
<pre>
  -h, --help    shows help message and exits
  -v, --version prints version information and exits
  -g, --global  此开关控制相关操作作用到系统环境变量
  -n, --name    环境变量名称 [default: "PATH"]
</pre>
### Examples:
1、删除名称为'JAVA_HOME'的系统变量
<pre>
  EnvVarEditor del -g -n JAVA_HOME
</pre>
