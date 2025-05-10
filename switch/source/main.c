#include <switch.h>
#include <Python.h>
#include <stdio.h>

u64 cur_progid = 0;
AccountUid userID={0};

#define MOD_DEF(ob, name, doc, methods) \
        static struct PyModuleDef moduledef = { \
          PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
        ob = PyModule_Create(&moduledef);

char python_error_buffer[0x400];

void show_error(const char* message, int exit)
{
    if (exit == 1) {
        Py_Finalize();
    }
    char* first_line = (char*)message;
    char* end = strchr(message, '\n');
    if (end != NULL)
    {
        first_line = python_error_buffer;
        memcpy(first_line, message, (end - message) > sizeof(python_error_buffer) ? sizeof(python_error_buffer) : (end - message));
        first_line[end - message] = '\0';
    }
    ErrorSystemConfig c;
    errorSystemCreate(&c, (const char*)first_line, message);
    errorSystemShow(&c);
    if (exit == 1) {
        Py_Exit(1);
    }
}

static PyObject* moduleImport(const char *name)
{
    PyObject* ob = PyImport_ImportModule(name);
    if (ob == NULL) {
        show_error(name, 1);
    }
    return ob;
}

static PyObject* commitsave(PyObject* self, PyObject* args)
{
    u64 total_size = 0;
    u64 free_size = 0;
    FsFileSystem* FsSave = fsdevGetDeviceFileSystem("save");

    FsSaveDataInfoReader reader;
    FsSaveDataInfo info;
    s64 total_entries=0;
    Result rc=0;
    
    fsdevCommitDevice("save");
    fsFsGetTotalSpace(FsSave, "/", &total_size);
    fsFsGetFreeSpace(FsSave, "/", &free_size);
    if (free_size < 0x800000) {
        u64 new_size = total_size + 0x800000;

        fsdevUnmountDevice("save");
        fsOpenSaveDataInfoReader(&reader, FsSaveDataSpaceId_User);

        while(1) {
            rc = fsSaveDataInfoReaderRead(&reader, &info, 1, &total_entries);
            if (R_FAILED(rc) || total_entries==0) break;

            if (info.save_data_type == FsSaveDataType_Account && userID.uid[0] == info.uid.uid[0] && userID.uid[1] == info.uid.uid[1] && info.application_id == cur_progid) {
                fsExtendSaveDataFileSystem(info.save_data_space_id, info.save_data_id, new_size, 0x400000);
                break;
            }
        }

        fsSaveDataInfoReaderClose(&reader);
        fsdevMountSaveData("save", cur_progid, userID);

    }
    return Py_None;
}

static PyObject* startboost(PyObject* self, PyObject* args)
{
    appletSetCpuBoostMode(ApmPerformanceMode_Boost);
    return Py_None;
}

static PyObject* disableboost(PyObject* self, PyObject* args)
{
    appletSetCpuBoostMode(ApmPerformanceMode_Normal);
    return Py_None;
}

static PyObject* restartprogram(PyObject* self, PyObject* args)
{
    appletRestartProgram(NULL, 0);
    return Py_None;
}

static PyMethodDef myMethods[] = {
    { "commitsave", commitsave, METH_NOARGS, "commitsave" },
    { "startboost", startboost, METH_NOARGS, "startboost" },
    { "disableboost", disableboost, METH_NOARGS, "disableboost" },
    { "restartprogram", restartprogram, METH_NOARGS, "restartprogram" },
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC PyInit__otrhlibnx(void)
{
    PyObject *m;
    MOD_DEF(m, "_otrhlibnx", "", myMethods)
    return m;
}

/*
PyMODINIT_FUNC PyInit_pygame_sdl2_color();
PyMODINIT_FUNC PyInit_pygame_sdl2_controller();
PyMODINIT_FUNC PyInit_pygame_sdl2_display();
PyMODINIT_FUNC PyInit_pygame_sdl2_draw();
PyMODINIT_FUNC PyInit_pygame_sdl2_error();
PyMODINIT_FUNC PyInit_pygame_sdl2_event();
PyMODINIT_FUNC PyInit_pygame_sdl2_gfxdraw();
PyMODINIT_FUNC PyInit_pygame_sdl2_image();
PyMODINIT_FUNC PyInit_pygame_sdl2_joystick();
PyMODINIT_FUNC PyInit_pygame_sdl2_key();
PyMODINIT_FUNC PyInit_pygame_sdl2_locals();
PyMODINIT_FUNC PyInit_pygame_sdl2_mouse();
PyMODINIT_FUNC PyInit_pygame_sdl2_power();
PyMODINIT_FUNC PyInit_pygame_sdl2_pygame_time();
PyMODINIT_FUNC PyInit_pygame_sdl2_rect();
PyMODINIT_FUNC PyInit_pygame_sdl2_render();
PyMODINIT_FUNC PyInit_pygame_sdl2_rwobject();
PyMODINIT_FUNC PyInit_pygame_sdl2_scrap();
PyMODINIT_FUNC PyInit_pygame_sdl2_surface();
PyMODINIT_FUNC PyInit_pygame_sdl2_transform();

PyMODINIT_FUNC PyInit__renpy();
PyMODINIT_FUNC PyInit__renpybidi();
PyMODINIT_FUNC PyInit_renpy_audio_renpysound();
PyMODINIT_FUNC PyInit_renpy_display_accelerator();
PyMODINIT_FUNC PyInit_renpy_display_render();
PyMODINIT_FUNC PyInit_renpy_display_matrix();
PyMODINIT_FUNC PyInit_renpy_gl_gl();
PyMODINIT_FUNC PyInit_renpy_gl_gldraw();
PyMODINIT_FUNC PyInit_renpy_gl_glenviron_shader();
PyMODINIT_FUNC PyInit_renpy_gl_glrtt_copy();
PyMODINIT_FUNC PyInit_renpy_gl_glrtt_fbo();
PyMODINIT_FUNC PyInit_renpy_gl_gltexture();
PyMODINIT_FUNC PyInit_renpy_pydict();
PyMODINIT_FUNC PyInit_renpy_style();
PyMODINIT_FUNC PyInit_renpy_styledata_style_activate_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_hover_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_idle_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_insensitive_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_selected_activate_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_selected_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_selected_hover_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_selected_idle_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_style_selected_insensitive_functions();
PyMODINIT_FUNC PyInit_renpy_styledata_styleclass();
PyMODINIT_FUNC PyInit_renpy_styledata_stylesets();
PyMODINIT_FUNC PyInit_renpy_text_ftfont();
PyMODINIT_FUNC PyInit_renpy_text_textsupport();
PyMODINIT_FUNC PyInit_renpy_text_texwrap();

// PyMODINIT_FUNC PyInit_renpy_compat_dictviews();
PyMODINIT_FUNC PyInit_renpy_gl2_gl2draw();
PyMODINIT_FUNC PyInit_renpy_gl2_gl2mesh();
PyMODINIT_FUNC PyInit_renpy_gl2_gl2mesh2();
PyMODINIT_FUNC PyInit_renpy_gl2_gl2mesh3();
PyMODINIT_FUNC PyInit_renpy_gl2_gl2model();
PyMODINIT_FUNC PyInit_renpy_gl2_gl2polygon();
PyMODINIT_FUNC PyInit_renpy_gl2_gl2shader();
PyMODINIT_FUNC PyInit_renpy_gl2_gl2texture();
PyMODINIT_FUNC PyInit_renpy_uguu_gl();
PyMODINIT_FUNC PyInit_renpy_uguu_uguu();

PyMODINIT_FUNC PyInit_renpy_lexersupport();
PyMODINIT_FUNC PyInit_renpy_display_quaternion();
*/

// Overide the heap initialization function.
void __libnx_initheap(void)
{
    void* addr = NULL;
    u64 size = 0;
    u64 mem_available = 0, mem_used = 0;

    svcGetInfo(&mem_available, InfoType_TotalMemorySize, CUR_PROCESS_HANDLE, 0);
    svcGetInfo(&mem_used, InfoType_UsedMemorySize, CUR_PROCESS_HANDLE, 0);

    if (mem_available > mem_used+0x200000)
        size = (mem_available - mem_used - 0x200000) & ~0x1FFFFF;
    if (size == 0)
        size = 0x2000000*16;

    Result rc = svcSetHeapSize(&addr, size);

    if (R_FAILED(rc) || addr==NULL)
        diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_HeapAllocFailed));

    extern char* fake_heap_start;
    extern char* fake_heap_end;

    fake_heap_start = (char*)addr;
    fake_heap_end   = (char*)addr + size;
}


Result createSaveData()
{
    NsApplicationControlData g_applicationControlData;
    size_t dummy;

    nsGetApplicationControlData(0x1, cur_progid, &g_applicationControlData, sizeof(g_applicationControlData), &dummy);

    FsSaveDataAttribute attr;
    memset(&attr, 0, sizeof(FsSaveDataAttribute));
    attr.application_id = cur_progid;
    attr.uid = userID;
    attr.system_save_data_id = 0;
    attr.save_data_type = FsSaveDataType_Account;
    attr.save_data_rank = 0;
    attr.save_data_index = 0;

    FsSaveDataCreationInfo crt;
    memset(&crt, 0, sizeof(FsSaveDataCreationInfo));
            
    crt.save_data_size = 0x800000;
    crt.journal_size = 0x400000;
    crt.available_size = 0x8000;
    crt.owner_id = g_applicationControlData.nacp.save_data_owner_id;
    crt.flags = 0;
    crt.save_data_space_id = FsSaveDataSpaceId_User;

    FsSaveDataMetaInfo meta={};

    return fsCreateSaveDataFileSystem(&attr, &crt, &meta);
}

void userAppInit()
{
    // fsdevUnmountAll();

    Result rc=0;
    PselUserSelectionSettings settings;
    
    rc = svcGetInfo(&cur_progid, InfoType_ProgramId, CUR_PROCESS_HANDLE, 0);
    rc = accountInitialize(AccountServiceType_Application);
    rc = accountGetPreselectedUser(&userID);
    if (R_FAILED(rc)) {
        s32 count;

        accountGetUserCount(&count);

        if (count > 1) {
            pselShowUserSelector(&userID, &settings);
        } else {
            s32 loadedUsers;
            AccountUid account_ids[count];
            accountListAllUsers(account_ids, count, &loadedUsers);
            userID = account_ids[0];
        }
    }

    if (accountUidIsValid(&userID)) {
        rc = fsdevMountSaveData("save", cur_progid, userID);
        if (R_FAILED(rc)) {
            rc = createSaveData();
            rc = fsdevMountSaveData("save", cur_progid, userID);
        }
    }

    romfsInit();
    socketInitializeDefault();
}

void userAppExit()
{
    fsdevCommitDevice("save");
    fsdevUnmountDevice("save");
    socketExit();
    romfsExit();
}


ConsoleRenderer* getDefaultConsoleRenderer(void)
{
    return NULL;
}


static AppletHookCookie applet_hook_cookie;
static void on_applet_hook(AppletHookType hook, void *param)
{
   switch (hook)
   {
      case AppletHookType_OnExitRequest:
        fsdevCommitDevice("save");
        svcSleepThread(1500000000ULL);
        appletUnlockExit();
        break;

      default:
         break;
   }
}


int main(int argc, char* argv[])
{
    setenv("MESA_NO_ERROR", "1", 1);

    appletLockExit();
    appletHook(&applet_hook_cookie, on_applet_hook, NULL);

    //Py_NoSiteFlag = 1;
    //Py_IgnoreEnvironmentFlag = 1;
    //Py_NoUserSiteDirectory = 1;
    //Py_DontWriteBytecodeFlag = 1;
    //Py_OptimizeFlag = 2;

    wchar_t* pyargs[] = {
        L"romfs:/Contents/renpy.py",
        NULL,
    };

    PyWideStringList argv_list = {.length = 1, .items = pyargs};

    PyStatus status;
    int python_result;

    show_error("before PyConfig_InitPythonConfig", 0);

    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    config.home = L"romfs:/Contents/lib.zip";
    config.site_import = 0;
    config.use_environment = 0;
    config.user_site_directory = 0;
    config.write_bytecode = 0;
    config.optimization_level = 2;
    config.parse_argv = 1;
    config.argv = argv_list;

    show_error("before PyRun_SimpleString", 0);

    python_result = PyRun_SimpleString("import sys; sys.path.insert('romfs:/Contents/lib.zip')\n");

    if (python_result == -1)
    {
        show_error("Could not set the Python path.\n\nThis is an internal error and should not occur during normal usage.", 1);
    }

    show_error("before PyConfig_SetBytesArgv", 0);

    /* Decode command line arguments.
       Implicitly preinitialize Python (in isolated mode). */
    status = PyConfig_SetBytesArgv(&config, argc, argv);
    if (PyStatus_Exception(status)) {
        goto exception;
    }

    static struct _inittab builtins[] = {
        {"_otrhlibnx", PyInit__otrhlibnx},
        {NULL, NULL}
    };

    show_error("before fopen", 0);

    FILE* sysconfigdata_file = fopen("romfs:/Contents/lib.zip", "rb");
    FILE* renpy_file = fopen("romfs:/Contents/renpy.py", "rb");

    if (sysconfigdata_file == NULL) {
        show_error("Could not find lib.zip.\n\nPlease ensure that you have extracted the files correctly so that the \"lib.zip\" file is in the same directory as the nsp file.", 1);
    }

    if (renpy_file == NULL) {
        show_error("Could not find renpy.py.\n\nPlease ensure that you have extracted the files correctly so that the \"renpy.py\" file is in the same directory as the nsp file.", 1);
    }

    fclose(sysconfigdata_file);

    //Py_SetPythonHome(L"romfs:/Contents/lib.zip");

    show_error("before PyImport_ExtendInittab", 0);

    if (PyImport_ExtendInittab(builtins) == -1) {
        show_error("PyImport_ExtendInittab", 0);
    }

    show_error("before Py_InitializeEx", 0);

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        goto exception;
    }
    PyConfig_Clear(&config);

    show_error("before moduleImport", 0);

    moduleImport("pygame_sdl2.color");
    moduleImport("pygame_sdl2.controller");
    moduleImport("pygame_sdl2.display");
    moduleImport("pygame_sdl2.draw");
    moduleImport("pygame_sdl2.error");
    moduleImport("pygame_sdl2.event");
    moduleImport("pygame_sdl2.gfxdraw");
    moduleImport("pygame_sdl2.image");
    moduleImport("pygame_sdl2.joystick");
    moduleImport("pygame_sdl2.key");
    moduleImport("pygame_sdl2.locals");
    moduleImport("pygame_sdl2.mouse");
    moduleImport("pygame_sdl2.power");
    moduleImport("pygame_sdl2.pygame_time");
    moduleImport("pygame_sdl2.rect");
    moduleImport("pygame_sdl2.render");
    moduleImport("pygame_sdl2.rwobject");
    moduleImport("pygame_sdl2.scrap");
    moduleImport("pygame_sdl2.surface");
    moduleImport("pygame_sdl2.transform");
    moduleImport("_renpy");
    moduleImport("_renpybidi");
    moduleImport("renpy.audio.renpysound");
    moduleImport("renpy.display.accelerator");
    moduleImport("renpy.display.matrix");
    moduleImport("renpy.display.render");
    moduleImport("renpy.gl.gldraw");
    moduleImport("renpy.gl.glenviron_shader");
    moduleImport("renpy.gl.glrtt_copy");
    moduleImport("renpy.gl.glrtt_fbo");
    moduleImport("renpy.gl.gltexture");
    moduleImport("renpy.pydict");
    moduleImport("renpy.style");
    moduleImport("renpy.styledata.style_activate_functions");
    moduleImport("renpy.styledata.style_functions");
    moduleImport("renpy.styledata.style_hover_functions");
    moduleImport("renpy.styledata.style_idle_functions");
    moduleImport("renpy.styledata.style_insensitive_functions");
    moduleImport("renpy.styledata.style_selected_activate_functions");
    moduleImport("renpy.styledata.style_selected_functions");
    moduleImport("renpy.styledata.style_selected_hover_functions");
    moduleImport("renpy.styledata.style_selected_idle_functions");
    moduleImport("renpy.styledata.style_selected_insensitive_functions");
    moduleImport("renpy.styledata.styleclass");
    moduleImport("renpy.styledata.stylesets");
    moduleImport("renpy.text.ftfont");
    moduleImport("renpy.text.textsupport");
    moduleImport("renpy.text.texwrap");
    moduleImport("renpy.gl2.gl2draw");
    moduleImport("renpy.gl2.gl2mesh");
    moduleImport("renpy.gl2.gl2mesh2");
    moduleImport("renpy.gl2.gl2mesh3");
    moduleImport("renpy.gl2.gl2model");
    moduleImport("renpy.gl2.gl2polygon");
    moduleImport("renpy.gl2.gl2shader");
    moduleImport("renpy.gl2.gl2texture");
    moduleImport("renpy.uguu.gl");
    moduleImport("renpy.uguu.uguu");
    moduleImport("renpy.lexersupport");
    moduleImport("renpy.display.quaternion");

    show_error("after moduleImport", 0);

    //PySys_SetArgvEx(1, pyargs, 1);

#define x(lib) \
    { \
        if (PyRun_SimpleString("import " lib) == -1) \
        { \
            show_error("Could not import python library " lib ".\n\nPlease ensure that you have extracted the files correctly so that the \"lib\" folder is in the same directory as the nsp file, and that the \"lib\" folder contains the folder \"python3.9\". \nInside that folder, the file \"" lib ".py\" or folder \"" lib "\" needs to exist.", 1); \
        } \
    }

    x("os");
    x("pygame_sdl2");
    x("encodings");

#undef x

    python_result = PyRun_SimpleFileEx(renpy_file, "romfs:/Contents/renpy.py", 1);

    if (python_result == -1)
    {
        show_error("An uncaught Python exception occurred during renpy.py execution.\n\nPlease look in the save:// folder for more information about this exception.", 1);
    }

    show_error("before Py_Exit", 0);

    Py_Exit(0);
    return 0;

exception:
    PyConfig_Clear(&config);
    if (PyStatus_IsExit(status)) {
        return status.exitcode;
    }

    show_error(status.err_msg, 0);

    /* Display the error message and exit the process with
       non-zero exit code */
    Py_ExitStatusException(status);
}
