# EnvVarEditor

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

## Command: set
### Usage: 
<pre>set [--help] [--version] [--global] [--name VAR] [--split] [--slim] [--sort-asc] [--sort-desc] [--value VAR] [--value+ VAR] [--value- VAR] [--save]</pre>

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
<pre>
  EnvVarEditor set -g -n JAVA_HOME -v c:\java
</pre>
