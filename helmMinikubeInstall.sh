docker build --no-cache ./mernapp -t mernapp:localbuild
minikube image load mernapp:localbuild
helm install km-mern-app -f myvalues.yaml bitnami/node
#kubectl port-forward --namespace default svc/km-mern-app-node 8080:80