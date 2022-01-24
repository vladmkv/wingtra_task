# Functional requirements
*Expected system behavior, functionality of subsystems, operations, data, workflows, roles. Maps to System, Integration, End to End, API testing.*

Application:
- Load multiple csv files
- Display features on map
- Edit, connect, measure features

Metadata module (MM):
- [MM1] Event triggered by user code is stored to local storage
- [MM2] Events are uploaded to server at specific periods
- [MM3] When offline, events are stored to local storage
- [MM4] When reconnected, pending events are sent to server on a regular basis
- [MM5] Custom event type can be specified
- [MM6] Configuration is specified in configuration file
- [MM7] Configuration can be specified on runtime
- [MM8] Local storage configuration includes location and maximum data size
- [MM9] Server configuration includes connection parameters
- [MM10] Server configuration includes upload periods

Each Functional Requirement MMx should map to specific acceptance test.

# Non-functional requirements
*User’s expectation, constraints on the design or implementation, system architecture*

- Platform compatibility: cross-platform
- Performance: event flow from 500 at once to one in 5 sec
- Extensibility: add new custom event types & sources
- Data integrity: offline storage of events & sync when back online. All reported events should be synchronized to server.
- Distribution: client-server
- Scalability: TBD
- Testability: TBD
- Fault tolerance: TBD
- Localization: TBD

# A high level solution description
- Metadata collection system includes multiple clients and a server
- Clients are arbitrary applications reporting specific events/actions through Metadata Module (MM)
- MM is a C++ thread-safe library providing client APIs for
    - registration to metadata reporting system
    - reporting events
    - connectivity & sync to Metadata Server (MS)
    - local data backup
- MM allows customization using configuration file an API for
    - MS connection & sync settings
    - local storage configuration
    - custom event type definition
- MS is web service implementing APIs for
    - client MM registration
    - event upload & DB storage

# Example code
## Client API
    // Metadata Client Service interface
    class IMCService
    {
    public:
        // Registration / deregistration
        virtual void StartSession() = 0;
        virtual void StopSession() = 0;
    
        // Report single event: can support more value types
        virtual void WriteEvent(const string &category, const string &eventName, const string &eventValue) = 0;

        // Report custom type event
        virtual void WriteCustomEvent(int type, const string &category, const string &eventName, const string &eventValue) = 0;

        // Push locally buffered events to Metadata server
        virtual void PushEvents() = 0;

        // Extra functionality like:
        // - Connectivity checks
        // - Settings API
        virtual void SetConfiguration(IMCConfiguration *config)
    };


