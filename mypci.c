#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>

#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/bus.h>

#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>


struct mypci_softc {
  device_t    mydev;
  struct cdev    *cdev;
};

static d_open_t    mypci_open;
static d_close_t   mypci_close;
static d_read_t    mypci_read;
static d_write_t   mypci_write;


static struct cdevsw mypci_cdevsw = {
    .d_version = D_VERSION,
    .d_open = mypci_open,
    .d_close = mypci_close,
    .d_write = mypci_write,
    .d_name = "mypci"
    };

