#include <switch.h>
#include <Python.h>
#include <stdio.h>

u64 cur_progid = 0;
AccountUid userID={0};

#define MOD_DEF(ob, name, doc, methods) \
        static struct PyModuleDef moduledef = { \
          PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
        ob = PyModule_Create(&moduledef);

#define MOD_IMPORT(name) \
        PyObject* ob = PyImport_ImportModule(name); \
        if (ob == NULL) show_error_and_exit("Could not import" name ".");

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

static PyMethodDef noMethods[] = {
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC PyInit__otrh_libnx(void)
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

char python_error_buffer[0x400];


void show_error_and_exit(const char* message)
{
    Py_Finalize();
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
    Py_Exit(1);
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

    Py_NoSiteFlag = 1;
    Py_IgnoreEnvironmentFlag = 1;
    Py_NoUserSiteDirectory = 1;
    Py_DontWriteBytecodeFlag = 1;
    Py_OptimizeFlag = 2;

    static struct _inittab builtins[] = {
        {"_otrhlibnx", PyInit__otrh_libnx},
        {NULL, NULL}
    };

    FILE* sysconfigdata_file = fopen("romfs:/Contents/lib.zip", "rb");
    FILE* renpy_file = fopen("romfs:/Contents/renpy.py", "rb");

    if (sysconfigdata_file == NULL)
    {
        show_error_and_exit("Could not find lib.zip.\n\nPlease ensure that you have extracted the files correctly so that the \"lib.zip\" file is in the same directory as the nsp file.");
    }

    if (renpy_file == NULL)
    {
        show_error_and_exit("Could not find renpy.py.\n\nPlease ensure that you have extracted the files correctly so that the \"renpy.py\" file is in the same directory as the nsp file.");
    }

    fclose(sysconfigdata_file);
    Py_InitializeEx(0);
    Py_SetPythonHome(L"romfs:/Contents/lib.zip");
    PyImport_ExtendInittab(builtins);
    MOD_IMPORT("pygame_sdl2.color");
    MOD_IMPORT("pygame_sdl2.controller");
    MOD_IMPORT("pygame_sdl2.display");
    MOD_IMPORT("pygame_sdl2.draw");
    MOD_IMPORT("pygame_sdl2.error");
    MOD_IMPORT("pygame_sdl2.event");
    MOD_IMPORT("pygame_sdl2.gfxdraw");
    MOD_IMPORT("pygame_sdl2.image");
    MOD_IMPORT("pygame_sdl2.joystick");
    MOD_IMPORT("pygame_sdl2.key");
    MOD_IMPORT("pygame_sdl2.locals");
    MOD_IMPORT("pygame_sdl2.mouse");
    MOD_IMPORT("pygame_sdl2.power");
    MOD_IMPORT("pygame_sdl2.pygame_time");
    MOD_IMPORT("pygame_sdl2.rect");
    MOD_IMPORT("pygame_sdl2.render");
    MOD_IMPORT("pygame_sdl2.rwobject");
    MOD_IMPORT("pygame_sdl2.scrap");
    MOD_IMPORT("pygame_sdl2.surface");
    MOD_IMPORT("pygame_sdl2.transform");
    MOD_IMPORT("_renpy");
    MOD_IMPORT("_renpybidi");
    MOD_IMPORT("renpy.audio.renpysound");
    MOD_IMPORT("renpy.display.accelerator");
    MOD_IMPORT("renpy.display.matrix");
    MOD_IMPORT("renpy.display.render");
    MOD_IMPORT("renpy.gl.gldraw");
    MOD_IMPORT("renpy.gl.glenviron_shader");
    MOD_IMPORT("renpy.gl.glrtt_copy");
    MOD_IMPORT("renpy.gl.glrtt_fbo");
    MOD_IMPORT("renpy.gl.gltexture");
    MOD_IMPORT("renpy.pydict");
    MOD_IMPORT("renpy.style");
    MOD_IMPORT("renpy.styledata.style_activate_functions");
    MOD_IMPORT("renpy.styledata.style_functions");
    MOD_IMPORT("renpy.styledata.style_hover_functions");
    MOD_IMPORT("renpy.styledata.style_idle_functions");
    MOD_IMPORT("renpy.styledata.style_insensitive_functions");
    MOD_IMPORT("renpy.styledata.style_selected_activate_functions");
    MOD_IMPORT("renpy.styledata.style_selected_functions");
    MOD_IMPORT("renpy.styledata.style_selected_hover_functions");
    MOD_IMPORT("renpy.styledata.style_selected_idle_functions");
    MOD_IMPORT("renpy.styledata.style_selected_insensitive_functions");
    MOD_IMPORT("renpy.styledata.styleclass");
    MOD_IMPORT("renpy.styledata.stylesets");
    MOD_IMPORT("renpy.text.ftfont");
    MOD_IMPORT("renpy.text.textsupport");
    MOD_IMPORT("renpy.text.texwrap");
    MOD_IMPORT("renpy.gl2.gl2draw");
    MOD_IMPORT("renpy.gl2.gl2mesh");
    MOD_IMPORT("renpy.gl2.gl2mesh2");
    MOD_IMPORT("renpy.gl2.gl2mesh3");
    MOD_IMPORT("renpy.gl2.gl2model");
    MOD_IMPORT("renpy.gl2.gl2polygon");
    MOD_IMPORT("renpy.gl2.gl2shader");
    MOD_IMPORT("renpy.gl2.gl2texture");
    MOD_IMPORT("renpy.uguu.gl");
    MOD_IMPORT("renpy.uguu.uguu");
    MOD_IMPORT("renpy.lexersupport");
    MOD_IMPORT("renpy.display.quaternion");

    wchar_t* pyargs[] = {
        L"romfs:/Contents/renpy.py",
        NULL,
    };

    PySys_SetArgvEx(1, pyargs, 1);

    int python_result;

    python_result = PyRun_SimpleString("import sys\nsys.path = ['romfs:/Contents/lib.zip']");

    if (python_result == -1)
    {
        show_error_and_exit("Could not set the Python path.\n\nThis is an internal error and should not occur during normal usage.");
    }

#define x(lib) \
    { \
        if (PyRun_SimpleString("import " lib) == -1) \
        { \
            show_error_and_exit("Could not import python library " lib ".\n\nPlease ensure that you have extracted the files correctly so that the \"lib\" folder is in the same directory as the nsp file, and that the \"lib\" folder contains the folder \"python2.7\". \nInside that folder, the file \"" lib ".py\" or folder \"" lib "\" needs to exist."); \
        } \
    }

    x("os");
    x("pygame_sdl2");
    x("encodings");

#undef x

    python_result = PyRun_SimpleFileEx(renpy_file, "romfs:/Contents/renpy.py", 1);

    if (python_result == -1)
    {
        show_error_and_exit("An uncaught Python exception occurred during renpy.py execution.\n\nPlease look in the save:// folder for more information about this exception.");
    }

    Py_Exit(0);
    return 0;
}
