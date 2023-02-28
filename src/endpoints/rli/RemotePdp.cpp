#include "RemotePdp.h"


RemotePdp::RemotePdp()
{
    parolModule = new RemoteParolModule();
    atcrbsSModule = new RemoteAtcrbsSModule();
}
