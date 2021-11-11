#ifndef IMAP_SERVER_H
#define IMAP_SERVER_H

//When compiled as a lib this is the end user interface
namespace MAP{
    class IMapServer{
        public:
        IMapServer();
        virtual ~IMapServer();
        //Configure(configModel)
        //AddCommands(vector of IMapCommand instances)
        //AddHook(hookname,handler(args))
        //RemoveHook(hookname);
    };
}

#endif