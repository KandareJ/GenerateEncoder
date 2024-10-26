message User {
    string userId = 1;
    string name = 2;
    list string aliases = 3;
    uint32 age = 4;
    double weight = 5;
    double height = 6;
}

message Notification {
    string notificationId = 1;
    string userId = 2;
    uint32 version = 3;
    uint64 timestamp = 4;
    int32 priority = 5;
    bool read = 6;
    bytes payload = 7;
}
