import datetime
from typing import ClassVar, Dict, List

class BagMetadata:
    bag_size: int
    compression_format: str
    compression_mode: str
    custom_data: Dict[str, str]
    duration: object
    files: List[FileInformation]
    message_count: int
    relative_file_paths: List[str]
    ros_distro: str
    starting_time: object
    storage_identifier: str
    topics_with_message_count: List[TopicInformation]
    version: int
    def __init__(self, version: int = ..., bag_size: int = ..., storage_identifier: str = ..., relative_file_paths: List[str] = ..., files: List[FileInformation] = ..., duration: object = ..., starting_time: object = ..., message_count: int = ..., topics_with_message_count: List[TopicInformation] = ..., compression_format: str = ..., compression_mode: str = ..., custom_data: Dict[str, str] = ..., ros_distro: str = ...) -> None: ...

class ConverterOptions:
    input_serialization_format: str
    output_serialization_format: str
    def __init__(self, input_serialization_format: str = ..., output_serialization_format: str = ...) -> None: ...

class Duration:
    def __init__(self, seconds: int, nanoseconds: int) -> None: ...

class FileInformation:
    duration: datetime.timedelta
    message_count: int
    path: str
    starting_time: object
    def __init__(self, path: str, starting_time: object, duration: object, message_count: int) -> None: ...

class MessageDefinition:
    encoded_message_definition: str
    encoding: str
    topic_type: str
    type_hash: str
    def __init__(self, topic_type: str, encoding: str, encoded_message_definition: str, type_hash: str) -> None: ...

class MetadataIo:
    def __init__(self) -> None: ...
    def deserialize_metadata(self, arg0: str) -> BagMetadata: ...
    def metadata_file_exists(self, arg0: str) -> bool: ...
    def read_metadata(self, arg0: str) -> BagMetadata: ...
    def serialize_metadata(self, arg0: BagMetadata) -> str: ...
    def write_metadata(self, arg0: str, arg1: BagMetadata) -> None: ...

class QoS:
    def __init__(self, history_depth: int) -> None: ...
    def avoid_ros_namespace_conventions(self, arg0: bool) -> QoS: ...
    def best_effort(self) -> QoS: ...
    def deadline(self, arg0: Duration) -> QoS: ...
    def durability(self, arg0: rmw_qos_durability_policy_t) -> QoS: ...
    def durability_volatile(self) -> QoS: ...
    def history(self, arg0: rmw_qos_history_policy_t) -> QoS: ...
    def keep_all(self) -> QoS: ...
    def keep_last(self, arg0: int) -> QoS: ...
    def lifespan(self, arg0: Duration) -> QoS: ...
    def liveliness(self, arg0: rmw_qos_liveliness_policy_t) -> QoS: ...
    def liveliness_lease_duration(self, arg0: Duration) -> QoS: ...
    def reliability(self, arg0: rmw_qos_reliability_policy_t) -> QoS: ...
    def reliable(self) -> QoS: ...
    def transient_local(self) -> QoS: ...

class ReadOrder:
    reverse: bool
    sort_by: ReadOrderSortBy
    def __init__(self, sort_by: ReadOrderSortBy = ..., reverse: bool = ...) -> None: ...

class ReadOrderSortBy:
    __members__: ClassVar[dict] = ...  # read-only
    File: ClassVar[ReadOrderSortBy] = ...
    PublishedTimestamp: ClassVar[ReadOrderSortBy] = ...
    ReceivedTimestamp: ClassVar[ReadOrderSortBy] = ...
    __entries: ClassVar[dict] = ...
    def __init__(self, value: int) -> None: ...
    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def name(self) -> str: ...
    @property
    def value(self) -> int: ...

class StorageFilter:
    exclude_service_events: List[str]
    exclude_topics: List[str]
    regex: str
    regex_to_exclude: str
    services_events: List[str]
    topics: List[str]
    def __init__(self, topics: List[str] = ..., services_events: List[str] = ..., regex: str = ..., exclude_topics: List[str] = ..., exclude_service_events: List[str] = ..., regex_to_exclude: str = ...) -> None: ...

class StorageOptions:
    custom_data: Dict[str, str]
    end_time_ns: int
    max_bagfile_duration: int
    max_bagfile_size: int
    max_cache_size: int
    snapshot_duration: object
    snapshot_mode: bool
    start_time_ns: int
    storage_config_uri: str
    storage_id: str
    storage_preset_profile: str
    uri: str
    def __init__(self, uri: str, storage_id: str = ..., max_bagfile_size: int = ..., max_bagfile_duration: int = ..., max_cache_size: int = ..., storage_preset_profile: str = ..., storage_config_uri: str = ..., snapshot_mode: bool = ..., snapshot_duration: object = ..., start_time_ns: int = ..., end_time_ns: int = ..., custom_data: Dict[str, str] = ...) -> None: ...

class TopicInformation:
    message_count: int
    topic_metadata: TopicMetadata
    def __init__(self, topic_metadata: TopicMetadata, message_count: int) -> None: ...

class TopicMetadata:
    id: int
    name: str
    offered_qos_profiles: List[QoS]
    serialization_format: str
    type: str
    type_description_hash: str
    def __init__(self, id: int, name: str, type: str, serialization_format: str, offered_qos_profiles: List[QoS] = ..., type_description_hash: str = ...) -> None: ...
    def equals(self, arg0: TopicMetadata) -> bool: ...

class rmw_qos_durability_policy_t:
    __members__: ClassVar[dict] = ...  # read-only
    RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT: ClassVar[rmw_qos_durability_policy_t] = ...
    RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL: ClassVar[rmw_qos_durability_policy_t] = ...
    RMW_QOS_POLICY_DURABILITY_UNKNOWN: ClassVar[rmw_qos_durability_policy_t] = ...
    RMW_QOS_POLICY_DURABILITY_VOLATILE: ClassVar[rmw_qos_durability_policy_t] = ...
    __entries: ClassVar[dict] = ...
    def __init__(self, value: int) -> None: ...
    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def name(self) -> str: ...
    @property
    def value(self) -> int: ...

class rmw_qos_history_policy_t:
    __members__: ClassVar[dict] = ...  # read-only
    RMW_QOS_POLICY_HISTORY_KEEP_ALL: ClassVar[rmw_qos_history_policy_t] = ...
    RMW_QOS_POLICY_HISTORY_KEEP_LAST: ClassVar[rmw_qos_history_policy_t] = ...
    RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT: ClassVar[rmw_qos_history_policy_t] = ...
    RMW_QOS_POLICY_HISTORY_UNKNOWN: ClassVar[rmw_qos_history_policy_t] = ...
    __entries: ClassVar[dict] = ...
    def __init__(self, value: int) -> None: ...
    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def name(self) -> str: ...
    @property
    def value(self) -> int: ...

class rmw_qos_liveliness_policy_t:
    __members__: ClassVar[dict] = ...  # read-only
    RMW_QOS_POLICY_LIVELINESS_AUTOMATIC: ClassVar[rmw_qos_liveliness_policy_t] = ...
    RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC: ClassVar[rmw_qos_liveliness_policy_t] = ...
    RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT: ClassVar[rmw_qos_liveliness_policy_t] = ...
    RMW_QOS_POLICY_LIVELINESS_UNKNOWN: ClassVar[rmw_qos_liveliness_policy_t] = ...
    __entries: ClassVar[dict] = ...
    def __init__(self, value: int) -> None: ...
    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def name(self) -> str: ...
    @property
    def value(self) -> int: ...

class rmw_qos_reliability_policy_t:
    __members__: ClassVar[dict] = ...  # read-only
    RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT: ClassVar[rmw_qos_reliability_policy_t] = ...
    RMW_QOS_POLICY_RELIABILITY_RELIABLE: ClassVar[rmw_qos_reliability_policy_t] = ...
    RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT: ClassVar[rmw_qos_reliability_policy_t] = ...
    RMW_QOS_POLICY_RELIABILITY_UNKNOWN: ClassVar[rmw_qos_reliability_policy_t] = ...
    __entries: ClassVar[dict] = ...
    def __init__(self, value: int) -> None: ...
    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def name(self) -> str: ...
    @property
    def value(self) -> int: ...

def convert_rclcpp_qos_to_rclpy_qos(arg0: QoS) -> object: ...
def get_default_storage_id() -> str: ...
def to_rclcpp_qos_vector(arg0: str, arg1: int) -> List[QoS]: ...
