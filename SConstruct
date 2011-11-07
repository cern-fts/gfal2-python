
import os

version= '2.0'
package_version = '1.4_preview'
license_type = "Apache Software License"


python_core=False
python_doc=False

pack_list = []
install_list = []
comp_list = []

if ARGUMENTS.get('python_core','no') =='yes':
	python_core=True
	
if ARGUMENTS.get('python_doc','no') =='yes':
	python_doc=True




env = Environment(tools=['default', 'packaging'], CPPFLAGS=["-Wall"])


builder = Builder(action = "cd ${TARGET.dir} && ln -s ${SOURCE.file} ${TARGET.file}", chdir = False)
env.Append(BUILDERS = {"Symlink" : builder})


# debug mode
if ARGUMENTS.get('debug','0') =='yes':
	print "DEBUG MODE"
	debug_mode = True
	env.Append(CPPFLAGS='-g')

# profile mode
if ARGUMENTS.get('profile','0') =='yes':
	print "PROFILE MODE"
	env.Append(CPPFLAGS='-pg', LINKFLAGS=['-pg'])
	
# prod mode
if ARGUMENTS.get('production','0') =='yes':
	print "prod MODE"
	env.Append(CPPFLAGS='-O3')







#packaging staff
libdir = (os.uname()[4] == 'x86_64') and "lib64" or "lib"


def arguments_to_str():
	ret = ' ';
	for arg, value in ARGUMENTS.iteritems():
		ret += arg+ '=' +value+ ' '
	return ret

def define_rpm_install(opt):
	return 'scons  '+ opt+ ' --install-sandbox="$RPM_BUILD_ROOT" install '

if(python_core):
	VariantDir('build24', 'src')
	VariantDir('build26', 'src')
	env_py24 = env.Clone()
	env_py26 = env.Clone()
	env_py24["python_version"] = 2.4
	env_py26["python_version"] = 2.6
	py24_main = SConscript("build24/SConscript", exports={'env' : env_py24})
	py26_main = SConscript("build26/SConscript", exports={'env' : env_py26})
	comp_list += [ py24_main, py26_main ]
	lib_main1 = env.Install('/usr/'+libdir+'/python2.4/site-packages/', py24_main)
	lib_main2 = env.Install('/usr/'+libdir+'/python2.6/site-packages/', py26_main)
	install_list += [lib_main1, lib_main2] 
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
			 source= [lib_main1, lib_main2] 
			 )
			 
if(python_doc):
	doc_files = Glob("example/python/*.py*");
	print doc_files
	examples = env.Install('/usr/share/gfal2-python/examples/', doc_files)
	install_list += [examples] 
	x_rpm_install = define_rpm_install(arguments_to_str());
	pack_list += env.Package( 
			 NAME     = 'gfal2-python-doc',
			 VERSION        = version,
			 PACKAGEVERSION = package_version,
			 PACKAGETYPE    = 'rpm',
			 LICENSE        = license_type,
			 SUMMARY        = 'doc for python bindings for gfal 2.0',
			 DESCRIPTION    = 'doc for python bindings for the grid file access library v2',
			 X_RPM_GROUP    = 'System Environment/Libraries',
			 X_RPM_INSTALL= x_rpm_install,
			 X_RPM_REQUIRES = 'gfal2-python',
			 source= [examples] 
			 )



env.Alias("install", install_list);

env.Alias("build", comp_list);

env.Alias("package_generator", pack_list);
