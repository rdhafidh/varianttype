import argparse
import os
import subprocess
import shutil
import platform
import sys


def logging(string):
    print(string)

enable_log=False
use_ninja=True

def convertToAbsolutePath(path):
    if path is None or not path or not os.path.exists(path):
        return ""
    if not os.path.isabs(path):
        return os.path.abspath(path)
    return path


def execCMD(cmd, use_shell, cw):
    try:
        p = subprocess.Popen(cmd, shell=use_shell, cwd=cw)
        p.communicate()
        retcode = p.wait()
        if retcode == 0:
            return True
        if retcode < 0:
            if enable_log:
                logging("Child was terminated by signal: " + -retcode)
    except BaseException:
        if enable_log:
            logging("Execution failed: " + cmd + " from cwd: " + cw)
    return False


def handleMkDirBuild(dir): 
    if os.path.exists(dir):
        return True
    try:
        os.mkdir(dir)
    except BaseException:
        if enable_log:
            logging("\nexception in handleMkDirBuild")
        return False
    
    if enable_log:
        logging("\nhandleMkDirBuild ok")
    return True


def doBuild(cmd, srcdir, cw): 
    if enable_log:
        logging("\ngenerated cmd: " +cmd)
        logging("\ngenerated cw: " +cw)
    if not handleMkDirBuild(cw):
        if enable_log:
            logging("\nhandleMkDirBuild failed")
        return False
    ret=execCMD(cmd, platform.system() != "Windows", cw)
    if not ret:
        if enable_log:
            logging("\nbuild failed: "+str(ret))
        return False
    bcmd=""
    if use_ninja or platform.system()=="Windows":   
        bcmd="ninja"
    else:
        bcmd="make -j3"
    ret=execCMD(bcmd,platform.system() != "Windows", cw)
    if not ret:
        if enable_log:
            logging("\nbuild failed: "+str(ret))
        return False
    return True
    
def runApp(clrootdir,apppath,appname,cw):
    cmd ="LD_LIBRARY_PATH="+clroot_dir+"/lib "
    cmd+=apppath+"/"+appname
    ret=execCMD(cmd,platform.system() != "Windows", cw)
    if not ret:
        if enable_log:
            logging("\nrun app failed somehow..")
        return False
    return True
    



sanitizer_check = ['address', 'undefined', 'leak']
parser = argparse.ArgumentParser()
parser.add_argument("--dir", default="", help="cmake source dir")
parser.add_argument("--name", default="", help="app name that is must be invoked")
parser.add_argument("--enable_log", help="turn on logging info",action="store_true")
parser.add_argument("--use_ninja", help="generate ninja build and use it instead of makefile",action="store_true")
parser.add_argument(
    "--clroot_dir", default="", help="default clang compiler sysroot dir")
parser.add_argument("--cmake_dir", default="", help="default cmake root dir")
parser.add_argument(
    "--build_release",
    help=
    "build release mode, instead of debug(effectivly disable all sanitizer check)",
    action="store_true")

args = parser.parse_args()
if not args.name:
    logging("require explicit appname to execute")
    sys.exit(-1)
path = convertToAbsolutePath(args.dir)
build_release = args.build_release
cmake_dir = args.cmake_dir
use_ninja=args.use_ninja
appname=args.name
enable_log=args.enable_log
clroot_dir = args.clroot_dir
if os.path.exists(path) and os.path.exists(clroot_dir):
    cmd = ""
    if not cmake_dir:
        cmd += "cmake "
    else:
        if platform.system() != "Windows": 
            cmd += cmake_dir + "/bin/cmake "
        else:
            cmd += cmake_dir + "\bin\cmake.exe "
    if build_release:
        cmd += "-DCMAKE_BUILD_TYPE=Release "
    else:
        cmd += "-DCMAKE_BUILD_TYPE=Debug "
    cmd += " "
    buildok = False
    if platform.system() != "Windows":
        if not build_release:
            if enable_log:
                logging("\nbuilding debug")
            for x in sanitizer_check:
                fcmd = cmd + "-DCMAKE_EXE_LINKER_FLAGS=\"-fsanitize="+x+" -L"+clroot_dir+"/lib -lgcc_s \" -DCMAKE_CXX_COMPILER=\"" + clroot_dir + "/bin/clang++\" -DCMAKE_C_COMPILER=\"" + clroot_dir + "/bin/clang\" -DCMAKE_C_FLAGS=\" -fno-omit-frame-pointer\" -DCMAKE_CXX_FLAGS=\"-fno-omit-frame-pointer -std=c++17 -fsanitize=" + x + "\" "
                if use_ninja:
                    fcmd += "-G Ninja "
                fcmd += path
                buildok = doBuild(fcmd, path, path + "/out")
                if not buildok:
                    break
                
                buildok=runApp(clroot_dir,path+"/out",appname,path+"/out")
                if not buildok:
                    break
                else:
                    if enable_log:
                        logging("\n[+] checked "+x+" done")
                    
        else:
            cmd += " -DCMAKE_CXX_FLAGS=\"-std=c++17\" " + path + "/out"
            buildok = doBuild(cmd, path, path + "/out")
    else:
        cmd += " -DCMAKE_CXX_FLAGS=\"/std:c++17\" " + path + "/out"
        buildok = doBuild(cmd, path, path + "/out")
    buildok = int(buildok)
    buildok -= 1
    if enable_log:
        logging("\nbuildok status: " + str(buildok))
    sys.exit(buildok)

else:
    parser.print_help()
    sys.exit(-1)
