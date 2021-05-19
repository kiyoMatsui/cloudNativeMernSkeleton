# cloudNativeMernSkeleton
A mern skeleton for deployment on kubernetes and development with docker-compose... well... debugging node in a container still has some unfortunate broken edge cases so it's still best to use the host pc and docker-compose for the dependent services.

## Introduction
This project demos a mern skeleton that can easily deploy on kubernetes with the bitnami/node and the nginx ingress controller Helm charts.
Docker-compose is used to run services for the dev environment.

Mern-skeleton taken from here (MIT): https://github.com/PacktPublishing/Full-Stack-React-Projects-Second-Edition/

## k8
Deploy with helm on a local kubernetes, like minikube, using the commands in helmMinikubeInstall.sh.
This way there is no need to own any online repos and images can be loaded into k8 manually.
Note, "minikube delete --all" does not remove the loaded image so "minikube cache delete mernapp:localbuild" needs to be run before redeploying a new image.