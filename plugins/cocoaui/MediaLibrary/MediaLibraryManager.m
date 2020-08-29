//
//  MediaLibraryManager.m
//  DeaDBeeF
//
//  Created by Alexey Yakovenko on 8/29/20.
//  Copyright © 2020 Alexey Yakovenko. All rights reserved.
//

#import "MediaLibraryManager.h"

extern DB_functions_t *deadbeef;

@interface MediaLibraryManager()

@property (nonatomic) ddb_medialib_plugin_t *medialibPlugin;
@property (nonatomic,readwrite) ddb_medialib_source_t source;

@end

@implementation MediaLibraryManager

- (instancetype)init
{
    self = [super init];
    if (!self) {
        return nil;
    }

    _medialibPlugin = (ddb_medialib_plugin_t *)deadbeef->plug_get_for_id ("medialib");
    _source = self.medialibPlugin->create_source ("deadbeef");

    return self;
}

- (void)dealloc
{
    if (_source) {
        _medialibPlugin->free_source(_source);
        _source = NULL;
    }
    _medialibPlugin = NULL;
}

@end
