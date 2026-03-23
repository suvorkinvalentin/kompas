const CACHE_NAME = 'compass-map-v1';

// Устанавливаем Service Worker
self.addEventListener('install', event => {
  console.log('[Service Worker] Установлен');
  self.skipWaiting();
});

// Активируем Service Worker и чистим старые кэши (если будут новые версии)
self.addEventListener('activate', event => {
  console.log('[Service Worker] Активирован');
  event.waitUntil(clients.claim());
});

// Перехватываем запросы (кэшируем тайлы карты и скрипты Leaflet)
self.addEventListener('fetch', event => {
  const url = new URL(event.request.url);

  // Если это запрос к картинкам карты (тайлам OpenStreetMap) или библиотеке Leaflet
  if (url.hostname.includes('tile.openstreetmap.org') || url.hostname.includes('unpkg.com')) {
    event.respondWith(
      caches.match(event.request).then(cachedResponse => {
        // Если нашли в кэше — отдаем из кэша (работает БЕЗ интернета)
        if (cachedResponse) {
          return cachedResponse;
        }
        // Если в кэше нет — скачиваем из интернета и СОХРАНЯЕМ в кэш на будущее
        return fetch(event.request).then(networkResponse => {
          return caches.open(CACHE_NAME).then(cache => {
            cache.put(event.request, networkResponse.clone());
            return networkResponse;
          });
        }).catch(() => {
          console.log('[Service Worker] Ошибка сети, тайл недоступен: ', event.request.url);
        });
      })
    );
  } else {
    // Для всех остальных запросов просто идем в сеть, как обычно
    event.respondWith(fetch(event.request).catch(() => {}));
  }
});