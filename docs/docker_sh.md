# Docker.sh
The main objective of this script is to be able to run `docker` commands simply and quickly. It makes use of the `.env` file to be able to directly identify `docker` image and container.

This is a brief explanation of the available commands.

Remember to give execute permissions to the file:
```sh
> chmod +x ./docker.sh
```
# Commands

## Help
This command will list the available commands.
```sh
> ./docker.sh help 
command is <help>
Available commands: 
 rmi 
 rmc 
 build 
 run 
 help 
 listcon 
 listimg 
```

## Remove Project Image
Force delete project image.
```sh
> ./docker.sh rmi 
command is <rmi>
Untagged: .env.IMAGE_NAME (Default: sat:1.0)
Deleted: sha256:732a8b97a86bf338044e6693ade78dab7a3cd2e8df0306cf34d5e9d4218f8601
Deleted: sha256:8f7915f0fb1676a9515ec90a280cf000615c65600794fcde392d1c056f93865f
Deleted: sha256:e53065dc7699d9a880399f7ea7c9dd7baa94f42ffe6db158db535bf8ea09862e
Deleted: sha256:2d0fc323b7910ecc701ccfa21c81beee45a6a2fe930c6d2b53c1a8fe2ad233d7
Deleted: sha256:6bdc42dba1fc73622c6fe804c7a18a0d83531fba1711ab4b734aab08a647b4e6
Deleted: sha256:514e67d43e358a8af5cf45ccb30378762231a8665b89a8b3e647743ce0fd7acb
Deleted: sha256:7a5082ee36b1ba9e8bd15eae497682b7e1d4ee8badf9766bc1ad9e87b994baa7
```

## Remove Project Container
Force delete project container.
```sh
> ./docker.sh rmc 
command is <rmc>
.env.CONTAINER_NAME (Default: sat_app)
```

## Build docker image
```sh
> ./docker.sh build 
command is <build>
...
```

## Run docker container
```sh
> ./docker.sh run 
command is <run>
root@3d7b38678779:/code# 
```

## List active images
```sh
> ./docker.sh listimg 
command is <listimg>
REPOSITORY       TAG       IMAGE ID       CREATED         SIZE
sat              1.0       1c2a8623f2f5   6 minutes ago   1.18GB
```

## List active containers
```sh
> ./docker.sh listcon 
command is <listcon>
CONTAINER ID   IMAGE     COMMAND    CREATED         STATUS                    PORTS     NAMES
1bf40251ae31   sat:1.0    "bash"    3 seconds ago   Exited (0) 1 second ago             sat_app
```
