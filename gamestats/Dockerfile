FROM bitnami/node:latest

COPY . /app

WORKDIR /app

ENV NODE_ENV=development

RUN npm install

EXPOSE 3000

CMD npm run start