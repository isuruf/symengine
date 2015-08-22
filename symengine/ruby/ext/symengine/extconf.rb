if File.exist?("../../../../CMakeLists.txt")
    dir = "../../../../"
else
    dir = "../../"
end
exec 'cmake -DCMAKE_INSTALL_PREFIX=%s \
            -DBUILD_TESTS=no \
            -DBUILD_BENCHMARKS=no \
            -DWITH_RUBY=yes %s %s;
         ' % [dir, ARGV.join(" "), dir]