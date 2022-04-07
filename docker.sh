#!/bin/bash

help (){
    printf "Available commands: \n rmi \n rmc \n build \n run \n help \n listcon \n listimg \n"
}

env_up (){
    local file_folder="$(cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )" # https://medium.com/@Aenon/bash-location-of-current-script-76db7fd2e388
    if [ -f "$file_folder/.env" ]; then
        set -a
        source "$file_folder/.env"   # https://gist.github.com/mihow/9c7f559807069a03e302605691f85572
        set +a
    else
        printf ".env file not found in project \n"
        return 1
    fi
}

run (){
    if [ ! "$(docker ps -aq -f name=${CONTAINER_NAME})" ]; then # https://stackoverflow.com/questions/38576337
        docker run --name ${CONTAINER_NAME} -v ${PWD}:/code --security-opt seccomp:unconfined -ti ${IMAGE_NAME}
    else
        docker start --interactive ${CONTAINER_NAME}  # https://stackoverflow.com/questions/34782678
    fi    
}

build () {
    docker build -t ${IMAGE_NAME} .
}

rmc () {
    docker rm --force ${CONTAINER_NAME}
}

rmi () {
    docker rmi --force ${IMAGE_NAME}
}

list_images () {
    docker images
}

list_containers () {
    docker container ps -a
}

main () {
    env_up
    command="$1"
    echo "command is <${command}>"
    case "${command}" in
        "build")
            build
            ;;
        "run")
            run
            ;;
        "rmc")
            rmc
            ;;
        "rmi")
            rmi
            ;;
        "listcon")
            list_containers
            ;;
        "listimg")
            list_images
            ;;
        "help")
            help
            ;;    
        *)
            printf "ERROR: Missing command. \n"
            help
            exit 1
            ;;
    esac
}

main "$@"