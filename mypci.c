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
    .d_read = mypci_read,
    .d_close = mypci_close,
    .d_write = mypci_write,
    .d_name = "mypci"
    };

static int
modevent( module_t mod __unused, int event, void *arg __unused)
{
  int error = 0;

  switch(event){
  case MOD_LOAD:
    uprintf("Mypci Device has loaded\n");
    break;
  case MOD_UNLOAD:
    uprintf("Mypci Dev has unloaded\n");
    break;
  default:
    error = EOPNOTSUPP;
    break;
  }
  return (error);
}

static devclass_t mypci_devclass;

static int
mypci_open( struct cdev *dev, int oflags, int devtype, struct thread *td)
{
  struct mypci_softc *sc;
  sc = dev->si_drv1;
  device_printf( sc->mydev, "Opened Successfully\n");
  return (0);
}

static int                                                                        
mypci_close( struct cdev *dev, int fflags, int devtype, struct thread *td)         
{                                                                                
  struct mypci_softc *sc;                                                        
  sc = dev->si_drv1;                                                             
  device_printf( sc->mydev, "Closed Successfully\n");                              
  return (0);                                                                    
}

static int
mypci_read( struct cdev *dev, struct uio *uio, int ioflag ){
  struct mypci_softc *sc;
  sc = dev->si_drv1;
  device_printf( sc->mydev, "Read request = %zd\n", uio->uio_resid );
  return (0);
}


static int                                                                       
mypci_write( struct cdev *dev, struct uio *uio, int ioflag ){                    
  struct mypci_softc *sc;                                                        
  sc = dev->si_drv1;                                                             
  device_printf( sc->mydev, "Write request = %zd\n", uio->uio_resid );             
  return (0);                                                                    
}

static struct _pcsid {
  uint32_t    type ;
  const char  *desc;
} pci_ids[] = {{0x1234abcd, "Red PCI Widget"},
               {0x4321fedc, "Blue PCI Widget"},
               {0x00000000, NULL}};

static int
mypci_identify( device_t dev ){ return  (0);}

static int
mypci_shutdown(device_t dev){ return (0);}

static int
mypci_suspend( device_t dev ) { return (0);}

static int
mypci_resume( device_t dev ) { return (0);} 

static int
mypci_probe( device_t dev )
{
  uint32_t type = pci_get_devid(dev);
  struct _pcsid *ep = pci_ids;

  while( ep->type && ep->type != type )
    ep++;
  if( ep->desc ) {
    device_set_desc( dev, ep->desc );
    return (BUS_PROBE_DEFAULT);
  }

  return (ENXIO);
}

static int
mypci_attach( device_t dev )
{
  struct mypci_softc *sc = device_get_softc( dev );
  int unit = device_get_unit( dev );
  sc->cdev = make_dev(&mypci_cdevsw, unit, UID_ROOT, GID_WHEEL,
		       0600, "mypci%d", unit);
  sc->cdev->si_drv1 = sc;

  return (0);
}

static int
mypci_detach( device_t dev )
{
  struct mypci_softc *sc = device_get_softc( dev );

  destroy_dev( sc->cdev );
  return (0);
}

static device_method_t mypci_methods[] = {
    /* Device Interface */

    DEVMETHOD(device_probe, mypci_probe),
    DEVMETHOD(device_attach, mypci_attach),
    DEVMETHOD(device_detach, mypci_detach),
    DEVMETHOD(device_identify, mypci_identify),
    DEVMETHOD(device_shutdown, mypci_shutdown),
    DEVMETHOD(device_resume, mypci_resume),
    DEVMETHOD(device_suspend, mypci_suspend),
    
    { 0, 0 }
};

static driver_t mypci_driver = {
  "mypci",
  mypci_methods,
  sizeof(struct mypci_softc)
};

DRIVER_MODULE( mypci, pci, mypci_driver, mypci_devclass, 0, 0 );
