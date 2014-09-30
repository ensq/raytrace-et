#include <stdafx.h>

#include <Et.h>
#include <windows.h>

static void onStateReceived(TX_HANDLE p_stateBag);
static void onFixationEvent(TX_HANDLE p_fixationDataBehavior);

static void TX_CALLCONVENTION cbOnEngineConnectionStateChanged(
                                                               TX_CONNECTIONSTATE p_connectionState,
                                                               TX_USERPARAM p_userParam);
static void TX_CALLCONVENTION cbOnPresenceStateChanged(
                                                       TX_CONSTHANDLE p_asyncData,
                                                       TX_USERPARAM p_userParam);
static void TX_CALLCONVENTION cbOnEvent(TX_CONSTHANDLE p_asyncData,
                                        TX_USERPARAM p_userParam);
static void TX_CALLCONVENTION cbOnSnapshotCommitted(TX_CONSTHANDLE p_asyncData,
                                                    TX_USERPARAM p_param);
    
static TX_CONTEXTHANDLE g_context;
static TX_HANDLE g_interactorSnapshot;
static TX_TICKET g_presenceStateChangedTicket;
static TX_TICKET g_connectionStateChangedTicket;

static EtState g_state;

HANDLE g_semaphore;

Et::Et() : m_interactorId("dv2520") {
    g_context = TX_EMPTY_HANDLE;
    g_interactorSnapshot = TX_EMPTY_HANDLE;

    g_presenceStateChangedTicket = TX_INVALID_TICKET;
    g_connectionStateChangedTicket = TX_INVALID_TICKET;

    g_semaphore = nullptr;
}
Et::~Et() {
    txDisableConnection(g_context);
    txUnregisterConnectionStateChangedHandler(g_context,
                                              g_connectionStateChangedTicket);
    txUnregisterStateChangedHandler(g_context, g_presenceStateChangedTicket);
    txShutdownContext(g_context, TX_CLEANUPTIMEOUT_DEFAULT, TX_FALSE);
    txReleaseObject(&g_interactorSnapshot);
    txReleaseContext(&g_context);

    assert(g_semaphore!=nullptr);
    CloseHandle(g_semaphore);
}

EtState Et::getState() {
    EtState state;
    BOOL wait = TRUE;
    DWORD waitResult;
    while(wait==TRUE) {
        waitResult = WaitForSingleObject(g_semaphore, 0L);
        switch(waitResult) {
        case WAIT_OBJECT_0: // thread was let through semaphore gate
            state = g_state;
            wait = FALSE;
            assert(ReleaseSemaphore(g_semaphore, 1, NULL));
            break;
        case WAIT_TIMEOUT:
            break;
        }
    }
    return state;
}

bool Et::init() {
    TX_RESULT isInitialized = txInitializeEyeX(
                                               TX_EYEXCOMPONENTOVERRIDEFLAG_NONE,
                                               NULL, NULL, NULL, NULL);
    if(isInitialized==TX_RESULT_OK) {
        isInitialized = txCreateContext(&g_context, TX_FALSE);
    }
    if(isInitialized==TX_RESULT_OK) {
        TX_HANDLE interactor = TX_EMPTY_HANDLE;
        TX_FIXATIONDATAPARAMS params = {
            TX_FIXATIONDATAMODE_SLOW
        };
        bool success = txCreateGlobalInteractorSnapshot(
                                                        g_context, m_interactorId,
                                                        &g_interactorSnapshot, &interactor)==TX_RESULT_OK;
        success &= txCreateFixationDataBehavior(interactor,
                                                &params)==TX_RESULT_OK;

        isInitialized = success==true ? TX_RESULT_OK : TX_RESULT_UNKNOWN;
        txReleaseObject(&interactor);
    }
    if(isInitialized==TX_RESULT_OK) {
        isInitialized = txRegisterConnectionStateChangedHandler(
                                                                g_context, &g_connectionStateChangedTicket,
                                                                cbOnEngineConnectionStateChanged, NULL);
    }
    if(isInitialized==TX_RESULT_OK) {
        isInitialized = txRegisterStateChangedHandler(
                                                      g_context, &g_presenceStateChangedTicket,
                                                      TX_STATEPATH_USERPRESENCE, cbOnPresenceStateChanged,
                                                      NULL);
    }
    TX_TICKET eventHandlerTicket = TX_INVALID_TICKET;
    if(isInitialized==TX_RESULT_OK) {
        isInitialized = txRegisterEventHandler(g_context,
                                               &eventHandlerTicket,
                                               cbOnEvent, NULL);
    }
    if(isInitialized==TX_RESULT_OK) {
        isInitialized = txEnableConnection(g_context);
    }
    if(isInitialized!=TX_RESULT_OK) {
        throw ExceptionDv2520("Failed to initialize EyeX context. Aborting.\n");
    } else {
#define CNT_START 1
#define CNT_MAX 1
        g_semaphore = CreateSemaphore(NULL, CNT_START, CNT_MAX, NULL);
        assert(g_semaphore!=nullptr);
    }

    return isInitialized==TX_RESULT_OK ? true : false;
}

void onStateReceived(TX_HANDLE p_stateBag) {
    // Establish whether or not the device itself is up-and-running:
    TX_INTEGER status;
    TX_RESULT success = txGetStateValueAsInteger(p_stateBag,
                                                 TX_STATEPATH_EYETRACKINGSTATE,
                                                 &status);
    assert(success==TX_RESULT_OK);
    switch(status) {
    case TX_EYETRACKINGDEVICESTATUS_TRACKING:
        OutputDebugString("The EyeX controller is up and tracking.\n");
        break;
    default:
        char str[256];
        sprintf_s(str, sizeof(str), "For some reason the EyeX controller is"
                  " not tracking. This may be due to the device itself not"
                  " being connected. Status code  %d. Aborting...\n", status);
        throw ExceptionDv2520(str);
        break;
    }

    // Retrieve screen size according to EyeX:
    TX_SIZE2 screenSize;
    success = txGetStateValueAsSize2(p_stateBag, TX_STATEPATH_DISPLAYSIZE, 
                                     &screenSize);
    assert(success==TX_RESULT_OK);
    char str[256];
    sprintf_s(str, sizeof(str), "EyeX interprets your screen size as"
              " %5.2f x %5.2f mm\n", screenSize.Width, screenSize.Height);
    OutputDebugString(str);

    // Retrieve screen dimensions according to EyeX:
    TX_SIZE2 screenDim;
    success = txGetStateValueAsSize2(p_stateBag, TX_STATEPATH_SCREENBOUNDS,
                                     &screenDim);
    assert(success==TX_RESULT_OK);
    sprintf_s(str, sizeof(str), "EyeX interprets your resolution as"
              " %5.0f x %5.0f pixels.\n", screenDim.Width, screenDim.Height);
    OutputDebugString(str),

        txReleaseObject(&p_stateBag);
}

void TX_CALLCONVENTION cbOnSnapshotCommitted(TX_CONSTHANDLE p_asyncData,
                                             TX_USERPARAM p_param) {
    TX_RESULT result = TX_RESULT_UNKNOWN;
    txGetAsyncDataResultCode(p_asyncData, &result);
    assert(result==TX_RESULT_OK || result==TX_RESULT_CANCELLED);
}

void TX_CALLCONVENTION cbOnEngineConnectionStateChanged(
                                                        TX_CONNECTIONSTATE p_connectionState,
                                                        TX_USERPARAM p_userParam) {
    if(p_connectionState==TX_CONNECTIONSTATE_CONNECTED) {
        TX_RESULT success = txCommitSnapshotAsync(g_interactorSnapshot,
                                                  cbOnSnapshotCommitted, NULL);
        assert(success==TX_RESULT_OK);

        TX_HANDLE stateBag = TX_EMPTY_HANDLE;
        char str[256];
        sprintf_s(str, sizeof(str), "Successfully initialized EyeX context;"
                  " application is thus connected to the EyeX Engine.\n");
        OutputDebugString(str);

        txGetState(g_context, TX_STATEPATH_EYETRACKING, &stateBag);
        onStateReceived(stateBag);
    }
}
void TX_CALLCONVENTION cbOnPresenceStateChanged(TX_CONSTHANDLE p_asyncData,
                                                TX_USERPARAM p_userParam) {
    TX_RESULT result = TX_RESULT_UNKNOWN;
    TX_HANDLE stateBag = TX_EMPTY_HANDLE;
    
    bool success = txGetAsyncDataResultCode(p_asyncData, &result)==TX_RESULT_OK
        && txGetAsyncDataContent(p_asyncData, &stateBag)==TX_RESULT_OK;
    if(success==true) {
        TX_INTEGER userPresence;
        result = txGetStateValueAsInteger(stateBag, TX_STATEPATH_USERPRESENCE, 
                                          &userPresence);
        if(result==TX_RESULT_OK) {
            if(userPresence!=TX_USERPRESENCE_PRESENT) {
                OutputDebugString("EyeX cannot recognize any present observer."
                                  " Searching...\n");
            }
        }
    }
}

void onFixationEvent(TX_HANDLE p_fixationDataBehavior) {
    TX_FIXATIONDATAEVENTPARAMS eventParams;
    TX_FIXATIONDATAEVENTTYPE eventType;
    char* eventDesc;

    TX_RESULT success = txGetFixationDataEventParams(p_fixationDataBehavior,
                                                     &eventParams);
    if(success==TX_RESULT_OK) {
        eventType = eventParams.EventType;
        switch(eventType) {
        case TX_FIXATIONDATAEVENTTYPE_DATA:
            eventDesc = "Data";
            break;
        case TX_FIXATIONDATAEVENTTYPE_END:
            eventDesc = "End";
            break;
        default:
            eventDesc = "Begin";
            break;
        }
        // char str[256];
        // sprintf_s(str, sizeof(str), "%.0f ms: [%.1f, %.1f] (Fixation %s)\n",
        //           eventParams.Timestamp, eventParams.X, eventParams.Y,
        //           eventDesc);
        // OutputDebugString(str);

        BOOL wait = TRUE;
        DWORD waitResult;
        while(wait==TRUE) {
            waitResult = WaitForSingleObject(g_semaphore, 0L);
            switch(waitResult) {
            case WAIT_OBJECT_0: // thread was let through semaphore gate
                g_state.x = eventParams.X;
                g_state.y = eventParams.Y;
                wait = FALSE;
                assert(ReleaseSemaphore(g_semaphore, 1, NULL));
                break;
            case WAIT_TIMEOUT:
                throw ExceptionDv2520("Semaphore timed out!\n");
                break;
            }
        }
    } else {
        throw ExceptionDv2520("Failed to interpret fixation event."
                              " Aborting...\n");
    }
}

void TX_CALLCONVENTION cbOnEvent(TX_CONSTHANDLE p_asyncData,
                                 TX_USERPARAM p_userParam) {
    TX_HANDLE event = TX_EMPTY_HANDLE;
    txGetAsyncDataContent(p_asyncData, &event);
    TX_HANDLE behavior = TX_EMPTY_HANDLE;
    TX_RESULT success = txGetEventBehavior(event, &behavior,
                                           TX_BEHAVIORTYPE_FIXATIONDATA);
    if(success==TX_RESULT_OK) {
        onFixationEvent(behavior);
        txReleaseObject(&behavior);
    }
    txReleaseObject(&event);
}
