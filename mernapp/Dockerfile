FROM bitnami/node:latest

COPY . /app

WORKDIR /app

ENV NODE_ENV=development
RUN npm install
RUN npm run build
ENV NODE_ENV=production

EXPOSE 3000

CMD npm run start