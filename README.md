# cloudNativeMernSkeleton
A mern skeleton for deployment on kubernetes and development with docker-compose.
Mern-skeleton taken from here (MIT): https://github.com/PacktPublishing/Full-Stack-React-Projects-Second-Edition/
This project demos a mern skeleton that can easily deploy on kubernetes with bitnami/node Helm chart.
docker-compose up to run a dev environment.
Deploy with helm on a local kubernetes, like minikube, using the commands in helmMinikubeInstall.sh.
This way there is no need to own any online repos and images can be loaded into k8 manually.
Note, "minikube delete --all" does not remove the loaded image so "minikube cache delete mernapp:localbuild" needs to be run before redeploying a new image.