FROM node:19-alpine

WORKDIR /usr/src/node/app

COPY package*.json ./

# install dependencies in folder above the app folder, to allow for volume mounting
RUN yarn install --modules-folder /usr/src/node/node_modules

COPY . .

CMD node client.js
