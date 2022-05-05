#ifndef PXR_ENUMS_H
#define PXR_ENUMS_H

typedef enum
{
    PXR_RET_SUCCESS = 0,
    PXR_RET_FAIL = -1,
    PXR_RET_INVALID_LAYER_ID = -2,
    PXR_RET_INVALID_IMAGE_INDEX = -3,
    PXR_RET_INPUT_POINTER_IS_NULL = -4,
    PXR_RET_INIT_SESSION_IS_NULL = -5,
    PXR_RET_INVALID_INPUT_PARAM = -6,
    PXR_RET_NOT_BEGIN_XR = -7,
}PxrReturnStatus;

typedef enum
{
    PXR_LAYER_PROJECTION = 0,
    PXR_LAYER_QUAD = 1,
    PXR_LAYER_CYLINDER = 2,
    PXR_LAYER_EQUIRECT = 3,
} PxrLayerShape;

typedef enum
{
    PXR_LAYER_LAYOUT_STEREO = 0,
    PXR_LAYER_LAYOUT_DOUBLE_WIDE = 1,
    PXR_LAYER_LAYOUT_ARRAY = 2,
    PXR_LAYER_LAYOUT_MONO = 3
}PxrLayerLayout;

typedef enum
{
    PXR_LAYER_FLAG_ANDROID_SURFACE = 1 << 0,
    PXR_LAYER_FLAG_PROTECTED_CONTENT = 1 << 1,
    PXR_LAYER_FLAG_STATIC_IMAGE = 1 << 2,
    PXR_LAYER_FLAG_USE_EXTERNAL_IMAGES = 1 << 4,
} PxrLayerCreateFlags;

typedef enum
{
    PXR_LAYER_FLAG_NO_COMPOSITION_DEPTH_TESTING = 1 << 3,
    PXR_LAYER_FLAG_USE_EXTERNAL_HEAD_POSE = 1 << 5,
    PXR_LAYER_FLAG_LAYER_POSE_NOT_IN_TRACKING_SPACE = 1 << 6,
    PXR_LAYER_FLAG_HEAD_LOCKED = 1 << 7,
    PXR_LAYER_FLAG_USE_EXTERNAL_IMAGE_INDEX = 1 << 8,
} PxrLayerSubmitFlags;

typedef enum
{
    PXR_FEATURE_MULTIVIEW = 0,
    PXR_FEATURE_FOVEATION = 1,
    PXR_FEATURE_EYETRACKING = 2
} PxrFeatureType;

typedef enum
{
    PXR_UNITY = 0,
    PXR_UNREAL = 1,
    PXR_NATIVE = 2
} PxrPlatformOption;

typedef enum
{
    PXR_OPENGL_ES = 0,
    PXR_VULKAN
} PxrGraphicOption;

typedef enum
{
    PXR_EYE_LEFT  = 0,
    PXR_EYE_RIGHT = 1,
    PXR_EYE_BOTH = 2,
    PXR_EYE_MAX = 2
} PxrEyeType;

typedef enum
{
    PXR_LOG_VERBOSE =2,
    PXR_LOG_DEBUG,
    PXR_LOG_INFO,
    PXR_LOG_WARN,
    PXR_LOG_ERROR,
    PXR_LOG_FATAL,
} PxrLogPriority;

typedef enum
{
    PXR_FOVEATION_LEVEL_NONE = -1,
    PXR_FOVEATION_LEVEL_LOW  = 0,
    PXR_FOVEATION_LEVEL_MID  = 1,
    PXR_FOVEATION_LEVEL_HIGH = 2,
    PXR_FOVEATION_LEVEL_TOP_HIGH = 3
} PxrFoveationLevel;

typedef enum
{
    PXR_COLOR_SPACE_LINEAR = 0,
    PXR_COLOR_SPACE_SRGB = 1
} PxrColorSpace;


typedef enum
{
    PXR_RENDER_TEXTURE_WIDTH = 0,
    PXR_RENDER_TEXTURE_HEIGHT,
    PXR_SHOW_FPS,
    PXR_RUNTIME_LOG_LEVEL,
    PXR_PXRPLUGIN_LOG_LEVEL,
    PXR_UNITY_LOG_LEVEL,
    PXR_UNREAL_LOG_LEVEL,
    PXR_NATIVE_LOG_LEVEL,
    PXR_TARGET_FRAME_RATE,
    PXR_NECK_MODEL_X,
    PXR_NECK_MODEL_Y,
    PXR_NECK_MODEL_Z,
    PXR_DISPLAY_REFRESH_RATE,
    PXR_ENABLE_6DOF,
    PXR_CONTROLLER_TYPE,
    PXR_PHYSICAL_IPD,
    PXR_TO_DELTA_SENSOR_Y,
    PXR_GET_DISPLAY_RATE,
    PXR_FOVEATION_SUBSAMPLED_ENABLED,
    PXR_TRACKING_ORIGIN_HEIGHT,
    PXR_ENGINE_VERSION,
    PXR_UNREAL_OPENGL_NOERROR,
    PXR_ENABLE_CPT,
    PXR_MRC_TEXTURE_ID,
    PXR_RENDER_FPS,
    PXR_MSAA_LEVEL_RECOMMENDED,
    PXR_MRC_TEXTURE_ID_2,
    PXR_SET_SURFACE_VIEW,
    PXR_API_VERSION,
    PXR_MRC_POSITION_Y_OFFSET,
    PXR_MRC_TEXTURE_WIDTH,
    PXR_MRC_TEXTURE_HEIGHT,
    PXR_SINGLEPASS,
    PXR_FOVLEVEL,
    PXR_ANDROID_SURFACE_DIMENSIONS
} PxrConfigType;

typedef enum
{
    PXR_RESET_POSITION = 0,
    PXR_RESET_ORIENTATION = 1,
    PXR_RESET_ORIENTATION_Y_ONLY= 2,
    PXR_RESET_ALL
} PxrResetSensorOption;

typedef enum
{
    PXR_TYPE_UNKNOWN = 0,
    PXR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING = 1,
    PXR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED = 2,
    PXR_TYPE_EVENT_DATA_EVENTS_LOST = 3,
    PXR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED = 4,
    PXR_TYPE_EVENT_DATA_PERF_SETTINGS_EXT = 5,
    PXR_TYPE_EVENT_DATA_CONTROLLER = 6,
    PXR_TYPE_EVENT_DATA_SESSION_STATE_READY=7,
    PXR_TYPE_EVENT_DATA_SESSION_STATE_STOPPING=8,
    PXR_TYPE_EVENT_DATA_SEETHROUGH_STATE_CHANGED = 9,
    PXR_TYPE_EVENT_HARDIPD_STATE_CHANGED = 10,
    PXR_TYPE_EVENT_FOVEATION_LEVEL_CHANGED = 11,
    PXR_TYPE_EVENT_FRUSTUM_STATE_CHANGED = 12,
    PXR_TYPE_EVENT_RENDER_TEXTURE_CHANGED = 13,
    PXR_TYPE_EVENT_TARGET_FRAME_RATE_STATE_CHANGED = 14,
    PXR_TYPE_EVENT_DATA_HMD_KEY = 15,
    PXR_TYPE_EVENT_DATA_MRC_STATUS = 16,
    PXR_TYPE_EVENT_DATA_REFRESH_RATE_CHANGED = 17,
    PXR_TYPE_EVENT_DATA_MAIN_SESSION_VISIBILITY_CHANGED_EXTX = 18
} PxrStructureType;

typedef enum
{
    PXR_EVENT_LEVEL_LOW = 0,
    PXR_EVENT_LEVEL_MID,
    PXR_EVENT_LEVEL_HIGH
} PxrEventLevel;

typedef enum
{
    PXR_SESSION_STATE_UNKNOWN = 0,
    PXR_SESSION_STATE_IDLE = 1,
    PXR_SESSION_STATE_READY = 2,
    PXR_SESSION_STATE_SYNCHRONIZED = 3,
    PXR_SESSION_STATE_VISIBLE = 4,
    PXR_SESSION_STATE_FOCUSED = 5,
    PXR_SESSION_STATE_STOPPING = 6,
    PXR_SESSION_STATE_LOSS_PENDING = 7,
    PXR_SESSION_STATE_EXITING = 8,
    PXR_SESSION_STATE_MAX_ENUM = 0x7FFFFFFF
} PxrSessionState;

typedef enum
{
    PXR_PERF_SETTINGS_DOMAIN_CPU = 1,
    PXR_PERF_SETTINGS_DOMAIN_GPU = 2,
    PXR_PERF_SETTINGS_DOMAIN_MAX_ENUM = 0x7FFFFFFF
} PxrPerfSettingsDomain;

typedef enum
{
    PXR_PERF_SETTINGS_SUB_DOMAIN_COMPOSITING = 1,
    PXR_PERF_SETTINGS_SUB_DOMAIN_RENDERING = 2,
    PXR_PERF_SETTINGS_SUB_DOMAIN_THERMAL = 3,
    PXR_PERF_SETTINGS_SUB_DOMAIN_MAX_ENUM = 0x7FFFFFFF
} PxrPerfSettingsSubDomain;

typedef enum
{
    PXR_PERF_SETTINGS_NOTIF_LEVEL_LOW = 0,
    PXR_PERF_SETTINGS_NOTIF_LEVEL_MID = 25,
    PXR_PERF_SETTINGS_NOTIF_LEVEL_HIGH = 75,
    PXR_PERF_SETTINGS_NOTIFICATION_LEVEL_MAX_ENUM = 0x7FFFFFFF
} PxrPerfSettingsNotificationLevel;

typedef enum
{
    PXR_PERF_SETTINGS_CPU = 1,
    PXR_PERF_SETTINGS_GPU = 2,
    PXR_PERF_SETTINGS_MAX_ENUM = 0x7FFFFFFF
} PxrPerfSettings;

typedef enum
{
    PXR_PERF_SETTINGS_LEVEL_POWER_SAVINGS = 0,
    PXR_PERF_SETTINGS_LEVEL_SUSTAINED_LOW = 25,
    PXR_PERF_SETTINGS_LEVEL_SUSTAINED_HIGH = 50,
    PXR_PERF_SETTINGS_LEVEL_BOOST = 75,
    PXR_PERF_SETTINGS_LEVEL_MAX_ENUM = 0x7FFFFFFF
} PxrPerfSettingsLevel;
/*
 device post event type pico add new device interface
*/
typedef enum
{
    PXR_DEVICE_CONNECTCHANGED = 0,
    PXR_DEVICE_MAIN_CHANGED = 1,
    PXR_DEVICE_VERSION = 2,
    PXR_DEVICE_SN = 3,
    PXR_DEVICE_BIND_STATUS = 4,
    PXR_STATION_STATUS = 5,
    PXR_DEVICE_IOBUSY = 6,
    PXR_DEVICE_OTASTAUS = 7,
    PXR_DEVICE_ID = 8,
    PXR_DEVICE_OTASATAION_PROGRESS = 9,
    PXR_DEVICE_OTASATAION_CODE = 10,
    PXR_DEVICE_OTACONTROLLER_PROGRESS = 11,
    PXR_DEVICE_OTACONTROLLER_CODE = 12,
    PXR_DEVICE_OTA_SUCCESS = 13,
    PXR_DEVICE_BLEMAC = 14,
    PXR_DEVICE_HANDNESS_CHANGED = 15,
    PXR_DEVICE_CHANNEL = 16,
    PXR_DEVICE_LOSSRATE = 17,
    PXR_DEVICE_THREAD_STARTED = 18,
    PXR_DEVICE_MENUPRESSED_STATE =19,
    PXR_DEVICE_HANDTRACKING_SETTING = 20
}PxrDeviceEventType;

typedef enum
{
    PXR_EYE_LEVEL = 0,
    PXR_FLOOR_LEVEL,
    PXR_STAGE_LEVEL
} PxrTrackingOrigin;

typedef enum
{
    PXR_CONTROLLER_LEFT = 0,
    PXR_CONTROLLER_RIGHT,
    PXR_CONTROLLER_COUNT
} PxrControllerHandness;

typedef enum
{
    PXR_HMD_3DOF = 0,
    PXR_HMD_6DOF
} PxrHmdDof;


typedef enum
{
    PXR_OVERLAY = 0,
    PXR_UNDERLAY
} PxrLayerType;

typedef enum
{
    PXR_SET_SEETHROUGH_VISIBLE = 0,
    PXR_SET_GUARDIANSYSTEM_DISABLE,
    PXR_RESUME_GUARDIANSYSTEM_FOR_STS,
    PXR_PAUSE_GUARDIANSYSTEM_FOR_STS,
    PXR_SHUTDOWN_SDK_GUARDIANSYSTEM,
    PXR_GET_CAMERA_DATA_EXT,
    PXR_START_SDK_BOUNDARY,
    PXR_SET_CONTROLLER_POSITION,  //unused
    PXR_START_CAMERA_PREVIEW,
    PXR_GET_ROOM_MODE_STATE,
    PXR_DISABLE_BOUNDARY,
    PXR_SET_MONO_MODE,
    PXR_GET_BOUNDARY_CONFIGURED,
    PXR_GET_BOUNDARY_ENABLED,
    PXR_SET_BOUNDARY_VISIBLE,
    PXR_SET_SEETHROUGH_BACKGROUND,
    PXR_GET_BOUNDARY_VISIBLE,
    PXR_GET_DIALOG_STATE,
    PXR_SET_SENSORLOST_CUSTOM_MODE,
    PXR_SET_SENSORLOST_CM_ST,
    PXR_GET_BOUNDARY_GEOMETRY_VEX_COUNT,

    PXR_RESET_TRACKING_HARD = 1000,
    PXR_GET_TRACKING_STATE = 1001,
} PxrFuncitonName;

typedef enum
{
    PXR_BOUNDARY_TEST_NODE_LEFT_HAND  = 0,
    PXR_BOUNDARY_TEST_NODE_RIGHT_HAND = 1,
    PXR_BOUNDARY_TEST_NODE_HEAD       = 2
}PxrBoundaryTestNode;

#endif  // PXR_ENUMS_H
