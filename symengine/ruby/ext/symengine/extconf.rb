exec 'cmake -DCMAKE_INSTALL_PREFIX=../../../../ \
            -DBUILD_TESTS=no \
            -DBUILD_BENCHMARKS=no \
            -DWITH_RUBY=yes %s ../../../../;
         ' % [ARGV.join(" ")]