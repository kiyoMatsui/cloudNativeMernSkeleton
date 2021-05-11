docker build --no-cache ./mernapp -t mernapp:localbuild
minikube image load mernapp:localbuild
helm install km-nginx bitnami/nginx-ingress-controller
kubectl apply -f ./tlssecret.yaml
kubectl apply -f ./mynginx.yaml
helm install km-mern-app -f myvalues.yaml bitnami/node
#minikube tunnel can be tricky on some platforms so the below may be better way to expose the nginx ingress controller to your local env.
minikube service --url km-nginx-nginx-ingress-controller