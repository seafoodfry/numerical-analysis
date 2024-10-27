FROM node:22-bookworm AS build

ARG SRC=stationary-phase

WORKDIR /app

# Install dependencies and build static files.
COPY ${SRC}/package.json ${SRC}/package-lock.json ./
RUN npm install
COPY ./${SRC} .
RUN npm run build


# Use Nginx for the final stage.
FROM nginx:1.27-bookworm AS serve

# Copy static build files from the build stage.
COPY --from=build /app/out /usr/share/nginx/html

# Expose the default Nginx port.
EXPOSE 80

# 7. Nginx runs by default; no need for additional CMD.