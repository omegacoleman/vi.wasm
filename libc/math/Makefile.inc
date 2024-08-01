mcommon_src = \
	math/s_finite.c \
	math/s_copysign.c \
	math/s_modf.c \
	math/s_scalbn.c \
	math/s_cbrt.c \
	math/s_exp10.c \
	math/s_expm1.c \
	math/s_ilogb.c \
	math/s_infinity.c \
	math/s_isinf.c \
	math/s_isinfd.c \
	math/s_isnan.c \
	math/s_isnand.c \
	math/s_log1p.c \
	math/s_nan.c \
	math/s_nextafter.c \
	math/s_pow10.c \
	math/s_rint.c \
	math/s_logb.c \
	math/s_log2.c \
	math/s_fdim.c \
	math/s_fma.c \
	math/s_fmax.c \
	math/s_fmin.c \
	math/s_fpclassify.c \
	math/s_lrint.c \
	math/s_llrint.c \
	math/s_lround.c \
	math/s_llround.c \
	math/s_nearbyint.c \
	math/s_remquo.c \
	math/s_round.c \
	math/s_scalbln.c \
	math/s_signbit.c \
	math/s_trunc.c \
	math/exp.c \
	math/exp2.c \
	math/exp_data.c \
	math/math_err.c \
	math/log.c \
	math/log_data.c \
	math/log2.c \
	math/log2_data.c \
	math/pow.c \
	math/pow_log_data.c

mcommon_fsrc = \
	math/sf_finite.c math/sf_copysign.c math/sf_modf.c math/sf_scalbn.c \
	math/sf_cbrt.c math/sf_exp10.c math/sf_expm1.c math/sf_ilogb.c \
	math/sf_infinity.c math/sf_isinf.c math/sf_isinff.c math/sf_isnan.c math/sf_isnanf.c \
	math/sf_log1p.c math/sf_nan.c math/sf_nextafter.c math/sf_pow10.c \
	math/sf_rint.c math/sf_logb.c \
	math/sf_fdim.c math/sf_fma.c math/sf_fmax.c math/sf_fmin.c math/sf_fpclassify.c \
	math/sf_lrint.c math/sf_llrint.c \
	math/sf_lround.c math/sf_llround.c math/sf_nearbyint.c math/sf_remquo.c math/sf_round.c \
	math/sf_scalbln.c math/sf_trunc.c \
	math/sf_exp.c math/sf_exp2.c math/sf_exp2_data.c math/sf_log.c math/sf_log_data.c \
	math/sf_log2.c math/sf_log2_data.c math/sf_pow_log2_data.c math/sf_pow.c \
	math/sinf.c math/cosf.c math/sincosf.c math/sincosf_data.c math/math_errf.c

libc_a_SOURCES += $(mcommon_src) $(mcommon_fsrc)

math_src = \
	math/k_standard.c math/k_rem_pio2.c \
	math/k_cos.c math/k_sin.c math/k_tan.c \
	math/e_acos.c math/e_acosh.c math/e_asin.c math/e_atan2.c \
	math/e_atanh.c math/e_cosh.c math/e_exp.c math/e_fmod.c \
	math/e_tgamma.c math/e_hypot.c math/e_j0.c \
	math/e_j1.c math/e_jn.c math/er_lgamma.c \
	math/e_log.c math/e_log10.c math/e_pow.c math/e_rem_pio2.c math/e_remainder.c \
	math/e_scalb.c math/e_sinh.c math/e_sqrt.c \
	math/w_acos.c math/w_acosh.c math/w_asin.c math/w_atan2.c \
	math/w_atanh.c math/w_cosh.c math/w_exp.c math/w_fmod.c \
	math/w_gamma.c math/wr_gamma.c math/w_hypot.c math/w_j0.c \
	math/w_j1.c math/w_jn.c math/w_lgamma.c math/wr_lgamma.c \
	math/w_log.c math/w_log10.c math/w_pow.c math/w_remainder.c \
	math/w_scalb.c math/w_sinh.c math/w_sqrt.c \
	math/w_sincos.c \
	math/w_drem.c \
	math/signgam.c \
	math/s_asinh.c math/s_atan.c math/s_ceil.c \
	math/s_cos.c math/s_erf.c math/s_fabs.c math/s_floor.c \
	math/s_frexp.c math/s_ldexp.c \
	math/s_signif.c math/s_sin.c \
	math/s_tan.c math/s_tanh.c \
	math/w_exp2.c math/w_tgamma.c

math_fsrc = \
	math/kf_rem_pio2.c \
	math/kf_cos.c math/kf_sin.c math/kf_tan.c \
	math/ef_acos.c math/ef_acosh.c math/ef_asin.c math/ef_atan2.c \
	math/ef_atanh.c math/ef_cosh.c math/ef_exp.c math/ef_fmod.c \
	math/ef_tgamma.c math/ef_hypot.c math/ef_j0.c \
	math/ef_j1.c math/ef_jn.c math/erf_lgamma.c \
	math/ef_log.c math/ef_log10.c math/ef_pow.c math/ef_rem_pio2.c math/ef_remainder.c \
	math/ef_scalb.c math/ef_sinh.c math/ef_sqrt.c \
	math/wf_acos.c math/wf_acosh.c math/wf_asin.c math/wf_atan2.c \
	math/wf_atanh.c math/wf_cosh.c math/wf_exp.c math/wf_fmod.c \
	math/wf_gamma.c math/wrf_gamma.c math/wf_hypot.c math/wf_j0.c \
	math/wf_j1.c math/wf_jn.c math/wf_lgamma.c math/wrf_lgamma.c \
	math/wf_log.c math/wf_log10.c math/wf_pow.c math/wf_remainder.c \
	math/wf_scalb.c math/wf_sinh.c math/wf_sqrt.c \
	math/wf_sincos.c \
	math/wf_drem.c \
	math/sf_asinh.c math/sf_atan.c math/sf_ceil.c \
	math/sf_cos.c math/sf_erf.c math/sf_fabs.c math/sf_floor.c \
	math/sf_frexp.c math/sf_ldexp.c \
	math/sf_signif.c math/sf_sin.c \
	math/sf_tan.c math/sf_tanh.c \
	math/wf_exp2.c math/wf_tgamma.c \
	math/wf_log2.c

libc_a_SOURCES += $(math_src) $(math_fsrc)
