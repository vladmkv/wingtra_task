# Functional requirements
Expected system behavior, functionality of subsystems, operations, data, workflows, roles. Maps to System, Integration, End to End, API testing.

Application:
- Load multiple csv files
- Display features on map
- Edit, connect, measure features

Metadata module (MM):
- [MM1] Events triggered by user actions
- [MM2] Events contain parameters with details
- [MM3] Story events locally
- [MM4] Upload events to server when online

# Non-functional requirements
User’s expectation, constraints on the design or implementation, system architecture

- Platform compatibility: cross-platform
- Throughput: event flow from 500 at once to one in 5 sec
- Extensibility: add new custom event types & sources
- Data integrity: offline storage of events & sync when back online. All reported events should be synchronized to server.
- Distribution: client-server
- Scalability: TBD
- Testability: TBD
- Fault tolerance: TBD
- Localization: TBD

# A high level description of your suggested solution.
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

        // Push locally buffered events to Metadata server
        virtual void PushEvents() = 0;

        // Extra functionality like:
        // - Connectivity checks
        // - Settings API
    };


