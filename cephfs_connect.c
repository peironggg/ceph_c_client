#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rados/librados.h>

int main(int argc, const char **argv)
{

  /* Declare the cluster handle and required arguments. */
  rados_t cluster;
  char cluster_name[] = "ceph";
  char user_name[] = "client.admin";
  uint64_t flags = 0;

  /* Initialize the cluster handle with the "ceph" cluster name and the "client.admin" user */
  int err;
  err = rados_create2(&cluster, cluster_name, user_name, flags);

  if (err < 0)
  {
    fprintf(stderr, "%s: Couldn't create the cluster handle! %s\n", argv[0], strerror(-err));
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("\nCreated a cluster handle.\n");
  }

  /* Read a Ceph configuration file to configure the cluster handle. */
  err = rados_conf_read_file(cluster, "/etc/ceph/ceph.conf");
  if (err < 0)
  {
    fprintf(stderr, "%s: cannot read config file: %s\n", argv[0], strerror(-err));
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("\nRead the config file.\n");
  }

  /* Connect to the cluster */
  err = rados_connect(cluster);
  if (err < 0)
  {
    fprintf(stderr, "%s: cannot connect to cluster: %s\n", argv[0], strerror(-err));
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("\nConnected to the cluster.\n");
  }

  /* Setup io context */
  rados_ioctx_t io;
  char *poolname = "cephfs.benchmark-fs.data";

  err = rados_ioctx_create(cluster, poolname, &io);
  if (err < 0)
  {
    fprintf(stderr, "%s: cannot open rados pool %s: %s\n", argv[0], poolname, strerror(-err));
    rados_shutdown(cluster);
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("\nCreated I/O context.\n");
  }

  /* Write data to the cluster synchronously. */
  err = rados_write(io, "hw", "Hello World!", 12, 0);
  if (err < 0)
  {
    fprintf(stderr, "%s: Cannot write object \"hw\" to pool %s: %s\n", argv[0], poolname, strerror(-err));
    rados_ioctx_destroy(io);
    rados_shutdown(cluster);
    exit(1);
  }
  else
  {
    printf("\nWrote \"Hello World\" to object \"hw\".\n");
  }
}