
import os

version= '2.0'
package_version = '1.1_preview'
license_type = "Apache Software License"


python_core=False

pack_list = []
install_list = []
comp_list = []

if ARGUMENTS.get('python_core','no') =='yes':
	python_core=True


env = Environment(tools=['default', 'packaging'])

builder = Builder(action = "cd ${TARGET.dir} && ln -s ${SOURCE.file} ${TARGET.file}", chdir = False)
env.Append(BUILDERS = {"Symlink" : builder})


# debug mode
if ARGUMENTS.get('debug','0') =='yes':
	print "DEBUG MODE"
	debug_mode = True
	env.Append(CFLAGS='-g')

# profile mode
if ARGUMENTS.get('profile','0') =='yes':
	print "PROFILE MODE"
	env.Append(CPPFLAGS='-pg', LINKFLAGS=['-pg'])
	
# prod mode
if ARGUMENTS.get('production','0') =='yes':
	print "prod MODE"
	env.Append(CPPFLAGS='-O3')


VariantDir('build', 'src')
py24_main= SConscript("build/SConscript", exports=['env'])


#packaging staff
libdir = (os.uname()[4] == 'x86_64') and "lib64" or "lib"


def arguments_to_str():
	ret = ' ';
	for arg, value in ARGUMENTS.iteritems():
		ret += arg+ '=' +value+ ' '
	return ret

def define_rpm_install(opt):
	return 'scons -j 8 '+ opt+ ' --install-sandbox="$RPM_BUILD_ROOT" "$RPM_BUILD_ROOT" '

if(python_core):
	comp_list += [ py24_main ]
	lib_main = env.Install('/usr/'+libdir+'/python2.4/site-packages/', py24_main)
	install_list += [lib_main] 
	x_rpm_install = define_rpm_install(arguments_to_str());
	pack_list += env.Package( 
			 NAME     = 'gfal2-python',
			 VERSION        = version,
			 PACKAGEVERSION = package_version,
			 PACKAGETYPE    = 'rpm',
			 LICENSE        = license_type,
			 SUMMARY        = 'Python bindings for gfal 2.0',
			 DESCRIPTION    = 'python bindings for the grid file access library v2',
			 X_RPM_GROUP    = 'System Environment/Libraries',
			 X_RPM_INSTALL= x_rpm_install,
			 X_RPM_POSTINSTALL = "ldconfig",
			 X_RPM_POSTUNINSTALL = "ldconfig",
			 X_RPM_REQUIRES = 'python, gfal2-core',
			 source= [lib_main] 
			 )



env.Alias("install", install_list);

env.Alias("build", comp_list);

env.Alias("package_generator", pack_list);
